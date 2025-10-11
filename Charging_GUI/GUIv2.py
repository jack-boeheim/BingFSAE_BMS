import tkinter as tk
from tkinter import ttk
from tkinter import scrolledtext
import serial.tools.list_ports
from serial import Serial
import threading
import datetime
import time

from pyCandapter import pyCandapter

NUM_MODULES = 8
NUM_CELLS_PER_MODULE = 12

NUM_COLUMNS = NUM_MODULES
NUM_ROWS = NUM_CELLS_PER_MODULE

INSTANTANEOUS_CELL_VOLTAGE = "Instantaneous Cell Voltage"
INTERNAL_RESISTANCE = "Internal Resistance"
OPEN_CIRCUIT_VOLTAGE = "Open Circuit Voltage"

CAN_MSG_PHRASE = "t0368".encode()  # Marker phrase to look for in serial data

CAN_BAUD_RATE = 250000

class SerialMonitor:
    def __init__(self, master):
        self.master = master
        self.window_width = self.master.winfo_screenwidth()  # Use full screen width
        self.window_height = self.master.winfo_screenheight()  # Use full screen height
        self.master.geometry(f'{self.window_width}x{self.window_height}')  # Set the window to full screen
        self.master.title("Serial Monitor")
        self.master.state('zoomed')  # Open in maximized state

        self.create_widgets()  # Call method to create widgets

        self.connection_active = False  # Flag to track connection state

    def create_widgets(self):
        # Port ComboBox Label
        self.label_port = ttk.Label(self.master, text="Select Port:")
        self.label_port.grid(row=0, column=0, padx=10, pady=10)

        #Find available comports
        ports = [port.device for port in serial.tools.list_ports.comports()]

        # Ports Combobox with available comports
        self.cb_ports = ttk.Combobox(self.master, values=ports, state="readonly")
        self.cb_ports.grid(row=0, column=0, padx=10, pady=10)

        # Baud ComboBox Label
        self.label_baud = ttk.Label(self.master, text="Select Baud Rate:")
        self.label_baud.grid(row=0, column=1, padx=10, pady=10)

        # Combobox with predefined baud rates
        self.cb_baud = ttk.Combobox(self.master, values=["2400","4800","9600","14400", "115200"], state="readonly")
        self.cb_baud.set("9600")  # Default to 9600 baud rate
        self.cb_baud.grid(row=0, column=2, padx=10, pady=10)

        # Connect Button
        self.btn_connect = ttk.Button(self.master, text="Connect", command=self.connect)
        self.btn_connect.grid(row=0, column=3, padx=10, pady=10)

        # Disconnect Button
        self.btn_disconnect = ttk.Button(self.master, text="Disconnect", command=self.disconnect, state=tk.DISABLED)
        self.btn_disconnect.grid(row=0, column=4, padx=10, pady=10)

        # Export TXT Button
        self.btn_export_text = ttk.Button(self.master, text="Export as TXT", command=self.export_txt, state=tk.DISABLED)
        self.btn_export_text.grid(row=0, column=5, padx=10, pady=10)

        # Combo box for ports
        self.cb_ports = ttk.Combobox(self.master, values=ports, state="readonly")
        self.cb_ports.grid(row=0, column=0, padx=10, pady=10)

        # Scrolled Text Area for logs
        self.text_log = scrolledtext.ScrolledText(self.master, wrap=tk.WORD, height=40, width = 150)
        self.text_log.grid(row=1, column=0, columnspan=18, padx=10, pady=10)

    def connect(self):

        port = self.cb_ports.get()  # Get selected port

        baud = int(self.cb_baud.get())  # Get selected baud rate

        self.candapter = pyCandapter(port, baud)

        self.candapter.closeCANBus()

        if self.candapter.openCANBus(CAN_BAUD_RATE):
            # Change button states after successful connection
            self.btn_disconnect["state"] = tk.NORMAL
            self.btn_connect["state"] = tk.DISABLED
            self.btn_export_text["state"] = tk.NORMAL
            self.connection_active = True

            # Start a new thread to read data from the serial port
            self.thread = threading.Thread(target=self.read_from_port)
            self.thread.start()

        else:
            self.text_log.insert(tk.END, f"Error Opening Can Bus Connection\n")

    def disconnect(self):
        self.candapter.closeCANBus()

        self.btn_connect["state"] = tk.NORMAL
        self.btn_disconnect["state"] = tk.DISABLED
        self.text_log.insert(tk.END, "Disconnected\n")

    def clear_matrix(self):
        self.matrix = [["NA"] * NUM_COLUMNS for _ in range(NUM_ROWS)]

    @staticmethod
    def verify_can_checksum(msg) -> bool:
        #need to implement
                
        for i in range(7):
            checksum += msg.data[i]

        if (checksum & 0xFF) == msg.data[7]:
            return True
        else:
            return False


    def read_from_port(self):
        columns = [f"Module {i + 1}" for i in range(NUM_COLUMNS)]  # Define columns
        self.tree = ttk.Treeview(self.master, columns=columns, show="headings")
        self.tree.grid(row=1, column=0, columnspan=8, sticky="nsew")

        # Define columns
        for col in columns:
            self.tree.heading(col, text=col)
            self.tree.column(col, stretch=False, anchor="center", width = int(self.window_width / NUM_COLUMNS))

        self.clear_matrix()  # Initialize matrix

        # Insert empty rows into the Treeview
        for row in self.matrix:
            self.tree.insert("", tk.END, values=row)

        while self.connection_active:

            msgs = []

            while self.candapter.device.in_waiting > 0: #If there are unread bytes
                msgs.append(self.candapter.readCANMessage())

            for msg in msgs:    

                data = msg.data

                #cell_id_hex = data[0]
                
                cell_id_decimal = data[0]

                # Calculate row and column indices
                row_index = cell_id_decimal % NUM_ROWS
                column_index = cell_id_decimal // NUM_ROWS

                #icv_hex = data[1:3]

                #Convert to decimal from hex (base 16)
                icv_decimal = (data[1]<<8) + data[2]

                #Convert using Orion specified conversion, round 3 decimal places
                icv_converted = round(((icv_decimal + 10000) * .00015), 3)
                dipslayed_value = icv_converted

                # Update matrix and Treeview
                if self.matrix[row_index][column_index] != dipslayed_value:
                    self.matrix[row_index][column_index] = dipslayed_value
                    self.update_cell(row_index, self.matrix[row_index])

            time.sleep(0.01)  # Small delay to reduce CPU usage

    def update_cell(self, row_index, row_values):
        """Update a specific row in the Treeview."""
        item_id = self.tree.get_children()[row_index]
        self.tree.item(item_id, values=row_values)

    def export_txt(self):
        filename = f"serial_log_{datetime.datetime.now().strftime('%Y%m%d%H%M%S')}.txt"
        with open(filename, "w") as file:
            file.write("Statistics Data:\n")
            stats = self.get_stat_data()
            for label, value in stats:
                file.write(f"{label} {value}\n")

            file.write("\nCell Data:\n")
            for child in self.tree.get_children():
                row_data = self.tree.item(child)["values"]
                file.write("\t".join(map(str, row_data)) + "\n")

        self.text_log.insert(tk.END, f"Data exported as TXT: {filename}\n")


# Main block
if __name__ == "__main__":
    root = tk.Tk()
    app = SerialMonitor(root)
    root.mainloop()
