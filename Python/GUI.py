import tkinter as tk
from tkinter import ttk
from tkinter import scrolledtext
import serial.tools.list_ports
from serial import Serial
import threading
import datetime
import time

class SerialMonitor:
    def __init__(self, master):
        self.master = master
        window_width = self.master.winfo_screenwidth()  # Use full screen width
        window_height = self.master.winfo_screenheight()  # Use full screen height
        self.master.geometry(f'{window_width}x{window_height}')  # Set the window to full screen
        self.master.title("Serial Monitor")
        self.master.state('zoomed')  # Open in maximized state

        self.create_widgets()  # Call method to create widgets

        self.connection_active = False  # Flag to track connection state

    def create_widgets(self):
        # Port ComboBox Label
        self.port_combobox_label = ttk.Label(self.master, text="Select Port:")
        self.port_combobox_label.grid(row=0, column=0, padx=10, pady=10)

        # Populate available ports into the ComboBox
        self.populate_ports()

        # Baud ComboBox Label
        self.baud_combobox_label = ttk.Label(self.master, text="Select Baud Rate:")
        self.baud_combobox_label.grid(row=0, column=1, padx=10, pady=10)

        # Combobox with predefined baud rates
        self.baud_combobox = ttk.Combobox(self.master, values=["2400","4800","9600","14400", "115200"], state="readonly")
        self.baud_combobox.set("9600")  # Default to 9600 baud rate
        self.baud_combobox.grid(row=0, column=2, padx=10, pady=10)

        # Connect Button
        self.connect_button = ttk.Button(self.master, text="Connect", command=self.connect)
        self.connect_button.grid(row=0, column=3, padx=10, pady=10)

        # Disconnect Button
        self.disconnect_button = ttk.Button(self.master, text="Disconnect", command=self.disconnect, state=tk.DISABLED)
        self.disconnect_button.grid(row=0, column=4, padx=10, pady=10)

        # Export TXT Button
        self.export_txt_button = ttk.Button(self.master, text="Export as TXT", command=self.export_txt, state=tk.DISABLED)
        self.export_txt_button.grid(row=0, column=5, padx=10, pady=10)

        # Scrolled Text Area for logs
        self.log_text = scrolledtext.ScrolledText(self.master, wrap=tk.WORD, width=205, height=40)
        self.log_text.grid(row=1, column=0, columnspan=18, padx=10, pady=10)

    def populate_ports(self):
        ports = [port.device for port in serial.tools.list_ports.comports()]
        self.port_combobox = ttk.Combobox(self.master, values=ports, state="readonly")
        self.port_combobox.grid(row=0, column=0, padx=10, pady=10)

    def connect(self):
        port = self.port_combobox.get()  # Get selected port
        baud = int(self.baud_combobox.get())  # Get selected baud rate
        try:
            self.ser = Serial(port, baud, timeout=1)  # Open serial connection
            self.connection_active = True

            # Change button states after successful connection
            self.disconnect_button["state"] = tk.NORMAL
            self.connect_button["state"] = tk.DISABLED
            self.export_txt_button["state"] = tk.NORMAL

            # Start a new thread to read data from the serial port
            self.thread = threading.Thread(target=self.read_from_port)
            self.thread.start()

            # Create Treeview Table to display data
            self.create_treeview_table()

        except Exception as e:
            self.log_text.insert(tk.END, f"Error: {str(e)}\n")

    def disconnect(self):
        self.connection_active = False  # Stop data reading
        if hasattr(self, 'ser') and self.ser.is_open:
            self.ser.close()  # Close serial port connection
        self.connect_button["state"] = tk.NORMAL
        self.disconnect_button["state"] = tk.DISABLED
        self.log_text.insert(tk.END, "Disconnected\n")

    def read_from_port(self):
        columns = [f"Module {i}" for i in range(1, 9)]  # Define columns
        self.tree = ttk.Treeview(self.master, columns=columns, show="headings")
        self.tree.grid(row=1, column=0, columnspan=8, sticky="nsew", padx=10, pady=10)

        # Define columns
        for col in columns:
            self.tree.heading(col, text=col)
            self.tree.column(col, stretch=True, anchor="center")  # Make columns stretchable

        rows, cols = 12, 8  # Define matrix size
        self.matrix = [["NA"] * cols for _ in range(rows)]  # Initialize matrix

        # Insert empty rows into the Treeview
        for row in self.matrix:
            self.tree.insert("", tk.END, values=row)

        phrase = "t0368".encode()  # Marker phrase to look for in serial data

        while self.connection_active:
            if self.ser.in_waiting > 0:
                data = self.ser.read(self.ser.in_waiting)  # Read all available data
                if phrase in data:
                    hex_cell = data[data.find(phrase) + len(phrase): data.find(phrase) + len(phrase) + 2]
                    decimal_cell = int(hex_cell, 16)

                    # Calculate row and column indices
                    row_index = decimal_cell // cols
                    column_index = decimal_cell % cols

                    hex_voltage = data[data.find(phrase) + len(phrase) + 2: data.find(phrase) + len(phrase) + 6]
                    decimal_voltage = int(hex_voltage, 16)
                    convert_to_volt = round(decimal_voltage * 0.0001, 3)

                    # Update matrix and Treeview
                    if self.matrix[row_index][column_index] != convert_to_volt:
                        self.matrix[row_index][column_index] = convert_to_volt
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

        self.log_text.insert(tk.END, f"Data exported as TXT: {filename}\n")

    def get_stat_data(self):
        stats = [
            ("Highest Resistance:", "4.08", "[001]"),
            ("Lowest Resistance:", "4.08", "[001]"),
            ("Avg Cell Resistance:", "4.08"),
            ("Delta Cell Resistance:", "0.00"),
            ("Highest Cell Volt:", "3.883", "[045]"),
            ("Lowest Cell Volt:", "3.868", "[064]"),
            ("Avg Cell Voltage:", "3.877"),
            ("Delta Cell Voltage:", "0.014"),
            ("Pack SOC:", "94.0%"),
            ("Pack Voltage:", "372.2V"),
            ("Temperature:", "20°C / 20°C"),
            ("Current Limits:", "+80A / -15A"),
            ("Relay Status:", "Discharge: ON, Charge: ON, Safety: OFF")
        ]
        flat_stats = []
        for stat in stats:
            label = stat[0]
            value = stat[1]
            extra = stat[2] if len(stat) > 2 else ""
            flat_stats.append((label, value + " " + extra))
        return flat_stats

# Main block
if __name__ == "__main__":
    root = tk.Tk()
    app = SerialMonitor(root)
    root.mainloop()
