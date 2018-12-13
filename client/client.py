import argparse
import npyscreen
import socket
import os
import time

from threading import Thread


DEFAULT_IP = "127.0.0.1"
DEFAULT_PORT = 9001
BUFFER_SIZE = 4096


class MultiLineEditBoxed(npyscreen.BoxTitle):
    """Wraps the editable text field inside a box to differentiate it from the chat log."""
    _contained_widget = npyscreen.MultiLineEdit


class ChatForm(npyscreen.ActionFormMinimal):

    # Change the text of the default OK button
    OK_BUTTON_TEXT = "Send"

    def create(self):
        # Initialize send_box first with a negative y value so that receive_box (which is
        # above send_box) can fill the terminal
        self.send_box = self.add(MultiLineEditBoxed, autowrap=False, max_height=5, rely=-7)
        self.receive_box = self.add(npyscreen.Pager, values=[], rely=1,
                                    max_height=self.curses_pad.getmaxyx()[0] - 7, editable=False, autowrap=True)

    def on_ok(self):
        """When user presses Send, this function sends the command to the server."""
        command = self.send_box.entry_widget.value.strip()
        if len(command) > BUFFER_SIZE:
            self.receive_box.values.append(
                'Sorry, your text cannot be greater than {} characters long. Please try again.'
                .format(BUFFER_SIZE))
        else:
            s.send(command.encode())
        self.send_box.entry_widget.value = ''



class ChatApplication(npyscreen.NPSAppManaged):
    def onStart(self):
        """Entry point for our terminal UI"""
        self.addForm('MAIN', ChatForm, name='BastionBuds Chat App')
        # Kick off the thread that listens for messages from the server
        listen_thread = Thread(target=listen, args=[self.getForm('MAIN'), s], daemon=True)
        listen_thread.start()

        # If there's a script, kick off a thread for that, too
        global args
        if args.script is not None:
            script_thread = Thread(target=execute_script, args=[self.getForm('MAIN'), s, args.script], daemon=True)
            script_thread.start()


def listen(chatForm, s):
    """Takes a ChatForm and a socket, then listens for messages forever"""

    exit_message = "Server disconnected. Please use Ctrl + C to quit out."
    while True:
        try:
            data = s.recv(BUFFER_SIZE)
        except:
            chatForm.receive_box.values.append(exit_message)
            return

        if not data:
            chatForm.receive_box.values.append(exit_message)
            return

        message = data.decode('utf-8')
        if message == "/QUIT":
            s.close()
            chatForm.receive_box.values.append(exit_message)
            return



        # display message to the user
        for line in message.splitlines():
            chatForm.receive_box.values.append(line)
            chatForm.receive_box.h_show_end(line)
        # force the terminal UI to refresh
        chatForm.display()


def execute_script(chatForm, s, script_fn):
    """Given a ChatForm, a socket, and the filename of a script, this function executes the script"""
    script = open(script_fn, 'r')

    for line in script.readlines():
        # Sleep a little to give time for UI to process and the like.
        time.sleep(1)

        if len(line) > BUFFER_SIZE:
            chatForm.receive_box.values.append(
                'Sorry, your text cannot be greater than {} characters long. Please try again.'
                .format(BUFFER_SIZE))
        else:
            s.send(line.strip().encode())

    script.close()


if __name__ == '__main__':
    # Parse any options/arguments passed to the client
    parser = argparse.ArgumentParser()
    parser.add_argument('--ip', default=DEFAULT_IP)
    parser.add_argument('--port', type=int, default=DEFAULT_PORT)
    parser.add_argument('--script', default=None)

    args = parser.parse_args()

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((args.ip, args.port))

    # Since this function blocks, everything else must be done in chatApp.onStart()
    chatApp = ChatApplication().run()
