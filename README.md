# ubntai
Ubuntu AI Assistant is your smart Linux companion, making troubleshooting and software installation effortless. No more digging through forums—just ask, and it finds and fixes issues for you. Whether you're a beginner or a pro, it streamlines your Ubuntu experience, ensuring a smoother, stress-free workflow.

# Framework: 
1) Database done (Mongo)
2) Djagno backend 
3) React frontend (Portfolio)
4) QT(Framework)
5) CPP 

# What implimented : 
1) Login and signup done 
2) Assistant 
3) Saving the history 

# What to impliment now : 
1) Rag for error solving 
2) Installation 
    * How to fetch the latest package and install it using apt package manager 
    * Installation and packages 
    
3) Creating a image of the working image 
4) Trouble Shooting and reading the errors 

# Troubleshoot : 
    1) Kernel error / Driver errors 
    2) Package error (Borken package) 
    3) Hardware errors (Software + hardware)
    4) 

# What else can we see in the package : 
1) Active processes in the system and packages using that.

# Different Log files : 
* All logs are present mostly in /var/log/
* Application installed from snap contains logs in snap/app/log
* For kernel logs run dmesg
* ~/.var/app/<app>/ for flatpack applications


# Add more feature : 
* Allow to see Failed login attems 
* Allow to see all the ports using net 
* Allow to see all the active ports in the system along with application name



# How Installation module will work : 
* User can search for any app 
* We will show where it is availalbe
* User can install by clicking over that app
### Snap packages : 
a) They have everything required to run inside them.
b) They include all dependencies.
c) So install directly using app 
### APT : 
### AppImage : 
* Runs on any linux distribution irrespective of the distribution.
* Download app from app image . 
* Make it executable and run the script ./app.appimage
* We won't facilitate app image because they are not centralized at one place.
### Ubuntu software center (App Store) 
### Flatpack 


# Lets apt : 
* It is a CLI frontend 

### Flow of installation package : 
* User Searches for package 
* Find the package
* See's where it is available 
* Now when use clicks on install/uninstall/ 
    * Open terminal behind the scenes.
    * Run the command on behalf of the user.
    * Shows the status.


* Reading installed applications
* applications installed using snap from /snap folder 
* 


# Checking for packages : 
* Binary Executables & System utilities : from usr/bin & /usr/sbin
* Applications : /snap,  /var/lib/flatpak/, 
* Libraries : /lib, /usr/lib / usr/share


# Types of files present in log folder : 
1) directory / can be found using isDir() return ture if dir()
2) .gz / to work on /
3) Log files / can be read easily 0 / can be found using isFile(); return true if file



# In a registration flow : 
    if user doesnot exists then only the new user is created and otp is sent else the user is logged in directly 
