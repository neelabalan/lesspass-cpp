# lesspass-cpp :key:
[![Total alerts](https://img.shields.io/lgtm/alerts/g/neelabalan/lesspass-cpp.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/neelabalan/lesspass-cpp/alerts/)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/neelabalan/lesspass-cpp.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/neelabalan/lesspass-cpp/context:cpp)

This an **Unoffical client** of [lesspass cli](https://github.com/lesspass/lesspass/tree/master/cli) in C++

What is LessPass?

> LessPass is a stateless password manager. Unlike other password managers which requires syncing, LessPass requires one master password to access all your passwords from anydevice. More on their LessPass [website](https://blog.lesspass.com/2016-10-19/how-does-it-work) on How it works.



## installation

```bash
# require cmake
git clone git@github.com:neelabalan/lesspass-cpp.git
cd lesspass-cpp
cmake .
make
# binary in bin folder in current directory
```



## usage

```bash
user@blue ~ ./bin/lesspass-cpp

Usage: lesspass-cpp [options]

Optional arguments:
-h --help       	show this help message and exit
--site          	site[Required]
--login         	login[Required]
--pass          	password[Required]
--counter       	counter (default is 5)
-l --no-lower   	exclude lowecase characters in password
-u --no-upper   	exclude uppercase characters in password
-d --no-digits  	exclude digits in password
-s --no-symbols 	exclude symbols in password
--length        	length of the password string (default is 10)

user@blue ~ ./lesspass-cpp --site google --login gmail.com --pass masterpassword
/0O@KG:Oio
```

