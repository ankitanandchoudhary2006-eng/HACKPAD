
import time
import board
import keypad
import usb_hid

from adafruit_hid.keyboard import Keyboard
from adafruit_hid.keycode import Keycode
from adafruit_hid.keyboard_layout_us import KeyboardLayoutUS

# ---------------------------------
# USB HID KEYBOARD
# ---------------------------------

keyboard = Keyboard(usb_hid.devices)
layout = KeyboardLayoutUS(keyboard)

# ---------------------------------
# MATRIX GPIO CONFIGURATION
# From your schematic
# ---------------------------------

ROWS = (
    board.GP28,  # R0
    board.GP0,   # R1
    board.GP6    # R2
)

COLS = (
    board.GP3,   # C0
    board.GP4,   # C1
    board.GP29   # C2
)

# If the switches do not register,
# try changing True to False.
keys = keypad.KeyMatrix(
    row_pins=ROWS,
    column_pins=COLS,
    columns_to_anodes=True
)

# ---------------------------------
# WINDOWS SHORTCUT FUNCTIONS
# ---------------------------------

def run_command(command):
    # Open Windows Run
    keyboard.send(Keycode.GUI, Keycode.R)
    time.sleep(0.35)

    # Enter the command
    layout.write(command)
    time.sleep(0.15)

    # Execute
    keyboard.send(Keycode.ENTER)
    time.sleep(0.5)


def open_chrome():
    run_command("chrome")


def open_bing():
    run_command("https://www.bing.com")


def open_explorer():
    keyboard.send(Keycode.GUI, Keycode.E)


def open_store():
    run_command("ms-windows-store:")


def open_recycle_bin():
    run_command("explorer.exe shell:RecycleBinFolder")


def open_downloads():
    run_command("explorer.exe shell:Downloads")


def open_settings():
    run_command("ms-settings:")


def open_task_manager():
    keyboard.send(
        Keycode.CONTROL,
        Keycode.SHIFT,
        Keycode.ESCAPE
    )


def open_notepad():
    run_command("notepad")


# ---------------------------------
# 3x3 BUTTON CONFIGURATION
# Row 0: Switch 1, 2, 3
# Row 1: Switch 4, 5, 6
# Row 2: Switch 7, 8, 9
# ---------------------------------

actions = [
    open_chrome,       # R0 C0
    open_bing,         # R0 C1
    open_explorer,     # R0 C2

    open_store,        # R1 C0
    open_recycle_bin,  # R1 C1
    open_downloads,    # R1 C2

    open_settings,     # R2 C0
    open_task_manager, # R2 C1
    open_notepad       # R2 C2
]

# ---------------------------------
# MAIN LOOP
# ---------------------------------

print("RP2040 Macro Pad Ready!")

while True:

    event = keys.events.get()

    if event and event.pressed:

        switch = event.key_number

        if 0 <= switch < len(actions):

            print("Switch pressed:", switch + 1)

            actions[switch]()

    time.sleep(0.01)
