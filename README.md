# SpotifyOrganizerV1
A way to provide locally stored structure to your Spotify playlists and albums. Uses Spotify API.

To use: Create a spotify developer account and create an app (https://developer.spotify.com/documentation/web-api/tutorials/getting-started). Once you have your client Id and client Secret, put them in the startup.h and curls_utils.h file in the default values. Also make sure you have added http://localhost:8888/callback to the app. Make sure all the libraries are installed and create a folder named musicData in the directory. Windows: I use gcc compiler with WSL (Ubuntu). To compiler: $ g++ main.cpp folderOperations.cpp curls_utils.cpp playlistOperations.cpp utility.cpp startup.cpp -o program -std=c++17 -lcurl -lmicrohttpd To run: $ ./program

This is a work in progress so more functionality will be added. I also hope to add the ability to see listening stats (as much as the Spotify API allows) and potentially a GUI (likely with SFML).
