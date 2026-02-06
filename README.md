# ftp-client
Simple ftp client with GUI written in C++ using SFML library.

This project is a simple to use ftp client with GUI.
When You compile it and run it,
<br>
You should see a window pop up with a main menu like this:
<br>
<br>
<img src="readme photos/mainMenu.png" height="320">

## Settings
When You click on settings button, You can change
<br>
Your resolution or set app to be in fullscreen mode.
<br>
<br>
<img src="readme photos/settings.png" height="320">
<br>
<br>
And that's all for settings. It's simple, isn't it? ^^.

## Connecting to FTP server
When You click on a connect button, You will see a
<br>
connection page with inputs to fill (just like a website form).
<br>
<br>
<img src="readme photos/connect.png" height="320">
<br>
<br>
You can click on anonymous login checkbox if You don't want to login 
<br>
with username and password and the server that You are connecting to,
<br>
supports anonymous users.
<br>
The first input is a ip/domain name input. It doesnt matter which
<br>
you type in there, the app will translate domain name to ip address.

## While being connected
After succesfully connecting to the FTP server, You will see a page like this:
<br>
<br>
<img src="readme photos/connected.png" height="320">
<br>
<br>
The connection stays active as long as You don't click disconnect button or close the application.
<br>
<br>
- On the left side there are local files and folders of Your current working directory<br>
and on the right side are the same things but on the FTP server.
  - On top of each other You can see Your current working<br>
  directory path for local machine and FTP server.
- By clicking on an file/folder, You can do several actions<br>
with it by using buttons at the bottom. You can:
  - delete FTP files and folders.
  - rename FTP files (if text input is filled in bottom left corner).
  - download FTP file to current working directory.
  - upload local file.
  - enter folder (changing current working directory).
- When inputing text to input in bottom left corner You can then:
  - change current local working directory.
  - create FTP folder.
  - rename selected FTP file.
- The last button disconnects You and ends connection.

## Error messages
They're simple and easy to read.
<br>
Example of error message:
<br>
<br>
<img src="readme photos/error.png" height="320">

## Additional informations
If You want to debug the application, You can change "DEBUG_VERSION" in "ConnectedState.cpp" from false to true.
<br>
Also to see the messages, You should change project property by right clicking on project in visual studio and selecting:
<br>
Properties > Linker > System
<br>
And changing Subsystem from SUBSYSTEM:WINDOWS to SUBSYSTEM:CONSOLE.
<br>
<br>
All textures are made by me using <a href="https://pixieditor.net">pixiEditor</a>
<br>
<a href="https://www.dafont.com/pixel-unicode.font">font</a>
<br>
<br>
Have fun :).
