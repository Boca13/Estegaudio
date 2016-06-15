# Estegaudio
A voice messages chat that allows user to inser hidden encripted files in audio in a unnoticeable way.

The project consists on three parts:
  - Presence server: Manages user login so they can connect to each other.
  - Windows Client: Desktop client used to audio chat between users. It has the possibility of including whole files inside the audio messages. The files are encripted end-to-end using a pre-shared private key. Later they are introduced in recorded audio using LSB (Least Significant Bit) encoding and sent to the peer. This inclusion is made in such a way that anybody sniffing traffic wouldn't notice.
  - Library: C++ lib used by the GUI client for core functions.
