﻿@page eclipse Eclipse Development Environment Setup

[TOC]

Integration with Eclipse simplifies the compilation, remote execution
and remote debugging of EGT applications using Buildroot.

The typical usage model of our EGT integration with Eclipse would be:

1. Creating the Buildroot system for a particular target board to execute
   or debug EGT applications or libraries. For this you must configure your
   Buildroot system, enable the EGT package with their dependencies and Build
   the Buildroot system. Refer to section @ref started_buildroot

2. Once Buildroot compilation is done. The output/image folder contains all
   the files to boot Linux on the target board. To create an sdcard image
   to boot Linux on the sam9x60EK board, refer to [create sdcard image](http://www.at91.com/linux4sam/bin/view/Linux4SAM/DemoSD).

3. Start Eclipse to develop, execute or debug your own custom EGT
   applications/libraries that will rely on the EGT library built and installed
   by Buildroot.

This tutorial will guide you through the installation and usage of Eclipse
for developing an EGT application using Buildroot.

@section eclipse_installation Eclipse Installation

At the current time, the latest stable version of Eclipse is called 2019-06.
Most Linux distributions come either with an Eclipse package or allow it to be
easily installed by the package manager in the distribution. However, note that
this is frequently an out of date version. You can download the latest version
from the central download site at http://www.eclipse.org. As an IDE, Eclipse supports
many different programming languages including Java, C, C++, Python, and many others.
At installation time, you need to select the version to support the package(s) that
you want. To make this easier, there is an installation tool which will guide you
through the steps.

@imageSize{eclipse_install.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_install.png "Eclipse installer"
@image latex eclipse/eclipse_install.png "Eclipse installer"

Download Eclipse IDE for C/C++ Developers and follow the steps below for
installation.

@see https://www.eclipse.org/downloads/packages/

Extract the eclipse-cpp-2019-06-R-linux-gtk-x86_64.tar.gz to folder /opt

@code{.sh}
sudo tar -zxvf eclipse-cpp-2019-06-R-linux-gtk-x86_64.tar.gz -C /opt

sudo chown -R root:root /opt/eclipse
@endcode

Create a desktop file and install it:

@code{.sh}
gedit eclipse.desktop
@endcode

copy the below lines to the eclipse.desktop file

@code{.sh}
[Desktop Entry]
Name=Eclipse
Type=Application
Exec=/opt/eclipse/eclipse
Terminal=false
Icon=eclipse
Comment=Integrated Development Environment
NoDisplay=false
Categories=Development;IDE;
Name[en]=Eclipse
@endcode

Set an executable permission and execute the following command to install it:

@code{.sh}
sudo desktop-file-install eclipse.desktop
@endcode

Create a symlink in /usr/local/bin

@code{.sh}
sudo ln -s /opt/eclipse/eclipse /usr/local/bin/eclipse
@endcode

For an eclipse icon to be displayed in dash, the eclipse icon can be added as:

@code{.sh}
sudo cp /opt/eclipse/icon.xpm /usr/share/pixmaps/eclipse.xpm
@endcode

Eclipse needs to have either OpenJDK or Sun Java installed to be able to
run eclipse. Here is a simple example of installing Open JDK 1.8:

@code{.sh}
sudo apt install openjdk-8-jdk
@endcode

With Eclipse installed in the /opt folder, it can now be accessed by any users.

@section start_eclipse_create_new_project Start Eclipse and Create an EGT Project

Start Eclipse from the Ubuntu Applications tray and you will be asked where to
place your workspace directory. By default, the location will be under the users
home directory, i.e. /home/\<user\>/eclipse-workspace.

@imageSize{eclipse_workspace.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_workspace.png "Eclipse workspace"
@image latex eclipse/eclipse_workspace.png "Eclipse workspace"

With the workspace set, click launch and Eclipse will start with a startup screen.

@imageSize{eclipse_startup_screen.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_startup_screen.png "Eclipse startup screen"
@image latex eclipse/eclipse_startup_screen.png "Eclipse startup screen"

Now we can create a new EGT project that will use a toolchain generated by Buildroot.
In the example below, we created an EGT project using autotool and it is up to the developer
to write the configure.ac file and Makefile.am. This allows the project to be built outside
Eclipse and to use standard Unix tools as your project build system. For more information on autotools,
refer to [GNU Autotools](https://www.gnu.org/software/automake/manual/html_node/Autotools-Introduction.html)

Click on "Create a new C/C++ Project" and Select "C++ Managed Build" in the New C/C++ Project window and then click Next.

@imageSize{eclipse_new_c_cpp_project.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_new_c_cpp_project.png "Select C++ Managed Build"
@image latex eclipse/eclipse_new_c_cpp_project.png "Select C++ Managed Build"

In the Create C/C++ Project window:

1. Set the project name.
2. In Project type, expand GNU Autotools and select Empty project.
3. In Toolchains, select GNU Autotools Toolchain.
4. Click on Next.

@imageSize{eclipse_cpp_project.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_cpp_project.png "Create C++ Project"
@image latex eclipse/eclipse_cpp_project.png "Create C++ Project"

To cross-compile this Project using the Buildroot toolchain, we need to set a few environment
variables and this can be done clicking "Advance Setting" in the Select Configurations window.

@imageSize{eclipse_cpp_project_select_configurations.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_cpp_project_select_configurations.png "C++ Project Select Configurations"
@image latex eclipse/eclipse_cpp_project_select_configurations.png "C++ Project Select Configurations"

In the Properties window, expand Autotools and select Configure Settings. Under Configure Settings,
select platform specifiers and set "Host platform (--host)" to "arm-buildroot-linux-gnueabi".

@imageSize{/eclipse_cpp_project_properties_autotools_configue.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_cpp_project_properties_autotools_configue.png  "C++ Project Properties Autotools"
@image latex eclipse/eclipse_cpp_project_properties_autotools_configue.png  "C++ Project Properties Autotools"

Now select Environment Variables and click New. Under the New Environment Variable window, set the
Buildroot toolchain path and Click OK.

1. Name: PATH
2. Value: buildroot/output/host/bin:$PATH

@imageSize{/eclipse_cpp_project_properties_autotools_env.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_cpp_project_properties_autotools_env.png  "C++ Project Properties Autotools Environment"
@image latex eclipse/eclipse_cpp_project_properties_autotools_env.png  "C++ Project Properties Autotools Environment"

To enable debug options, select Advance and check the Debug(-g) option and then click Apply.

@imageSize{/eclipse_cpp_project_properties_autotools_advance.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_cpp_project_properties_autotools_advance.png  "C++ Project Properties Autotools Advance"
@image latex eclipse/eclipse_cpp_project_properties_autotools_advance.png  "C++ Project Properties Autotools Advance"

Now expand the C/C++ Build. Select Environment and click Add. In the New Variable window, set the
Buildroot toolchain path using the below details. Click OK and "Apply and Close" and then click Finish.

1. Name: PATH
2. Value: buildroot/output/host/bin:$PATH
3. Check Add to all configurations

@imageSize{/eclipse_cpp_project_properties_build_env.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_cpp_project_properties_build_env.png  "C++ Project Properties Build Environment"
@image latex eclipse/eclipse_cpp_project_properties_build_env.png  "C++ Project Properties Build Environment"

Create source files in the Project by right Clicking on the Project name and select New -> Source File.

@imageSize{eclipse_new_source_file.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_new_source_file.png "New C++ Source file"
@image latex eclipse/eclipse_new_source_file.png "New C++ Source File"

In the New Source File window, set the File name basic.cpp and click Finish

@imageSize{eclipse_new_file_basic.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_set_filename.png "Enter filename"
@image latex eclipse/eclipse_set_filename.png "Enter filename"

Copy the lines below to basic.cpp file:

@snippet "../examples/basic/basic.cpp" Example

@imageSize{eclipse_new_cpp.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_new_cpp.png "Create cpp file"
@image latex eclipse/eclipse_new_cpp.png "Create cpp file"

Similarly, create a new configure.ac file and copy the below lines:

@code{.sh}
AC_PREREQ(2.59)
AC_INIT(basic, 1.0)
AC_PROG_CXX

AM_INIT_AUTOMAKE([1.10 foreign])

PKG_CHECK_MODULES(EGT, [libegt])
AC_SUBST(EGT_LIBS)
AC_SUBST(EGT_CFLAGS)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
@endcode

@imageSize{eclipse_new_configure.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_new_configure.png "Create configure.ac file"
@image latex eclipse/eclipse_new_configure.png "Create configure.ac file"

Now create a new Makefile.am file and copy the below lines:

@code{.sh}
bin_PROGRAMS=basic

basic_SOURCES=basic.cpp
basic_CPPFLAGS = $(EGT_CFLAGS)
basic_LDADD = $(EGT_LIBS) -lpthread
@endcode

@imageSize{eclipse_new_Makefile_am.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_new_Makefile_am.png "Create Makefile.am file"
@image latex eclipse/eclipse_new_Makefile_am.png "Create Makefile.am file"

Save all using file->saveall or press (shift + Ctrl + S).

To Build this Project, first invoke the Autoreconf tool by right clicking on the
project and selecting Invoke Autotools->Invoke Autoreconf. This operation will
call the autotools to generate the configure script and Makefile.in files. This
operation has to be done again if you make changes to configure.ac or Makefile.am.

@imageSize{eclipse_invoke_autoreconf.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_invoke_autoreconf.png "Invoke autoreconf"
@image latex eclipse/eclipse_invoke_autoreconf.png "Invoke autoreconf"

Set the "-i -v -f" option to the autoreconf window and click OK. This tells
autoreconf to automatically install additional scripts and files that
are needed for correct operation of the configure script.

@imageSize{eclipse_autoreconf_args.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_autoreconf_args.png "Set autoreconf arguments"
@image latex eclipse/eclipse_autoreconf_args.png "Set autoreconf arguments"

Now we can see in the console that the autoreconf command finished successfully and the
configure and Makefile.in files have generated in the project folder.

@imageSize{eclipse_autoreconf_success.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_autoreconf_success.png "Autoreconf success"
@image latex eclipse/eclipse_autoreconf_success.png "Autoreconf success"

Now reconfigure the project by right clicking on the project and selecting Reconfigure Project.
This will call the configure script with the argument --host option and set Environment variable
values to ensure that your project will be cross-compiled with the Buildroot toolchain.
You may define additional options in the project properties.

@imageSize{eclipse_reconfigure_project.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_reconfigure_project.png "Reconfigure project"
@image latex eclipse/eclipse_reconfigure_project.png "Reconfigure project"

In the console, we can see that the operation finished successfully and Makefile generated in
the Project folder.

@imageSize{eclipse_reconfigure_success.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_reconfigure_success.png "Reconfigure project finished"
@image latex eclipse/eclipse_reconfigure_success.png "Reconfigure project finished"

Finally, to build this project, select Project-> "Build All" or Press (Ctrl-b).

@imageSize{eclipse_build_success.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_build_success.png "Build project"
@image latex eclipse/eclipse_build_success.png "Build project"

@section eclipse_execute_egt  Executing EGT application on a Target Remotely

Eclipse will execute an EGT application on a target board by transfering the
application binary using SSH. SSH server must have been installed on the target board.

Buildroot offers you two choices:

1. The Dropbear package can be enabled in Networking applications -> dropbear.
2. The full-blown SSH implementation, OpenSSH, can be enabled in Networking applications -> openssh.

Most SSH servers do not allow log in as root with an empty password. To set the root password, log in
on the target board and type the below commands:

@code{.sh}
# passwd
Changing password for root
New password:
Bad password: too short
Retype password:
Password for root changed by root
@endcode

Type the below commands to configure the Ethernet interface and to make sure the host machine and target board
are connected.

@code{.sh}
# ifconfig eth0 192.168.1.12 up
@endcode

The first step is to create a new run configuration. Right click your EGT project and click on Run As->Run Configurations...

@imageSize{eclipse_run_as.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_run_as.png "Run as"
@image latex eclipse/eclipse_run_as.png "Run as"

Double click on "C/C++ Remote Application" to create a Run configuration for the target board.

@imageSize{eclipse_run_create_new.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_run_create_new.png "New launch configuration"
@image latex eclipse/eclipse_run_create_new.png "New launch configuration"

To setup an ssh connection with the target, click “New” under connection and select SSH and click on Okay.

@imageSize{eclipse_run_ssh_conn.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_run_ssh_conn.png "SSH connection setup"
@image latex eclipse/eclipse_run_ssh_conn.png "SSH connection setup"

In the New Connection window, set the below details and click Finish.

1. Connection Name: Remote Host
2. Host: IP Address of a target board.
3. User: root
4. Check public key based authentication.
5. passphrase: root password of target board.

@imageSize{eclipse_run_conn_finish.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_run_conn_finish.png "Finish launch configuration"
@image latex eclipse/eclipse_run_conn_finish.png "Finish launch configuration"

Set the Remote Absolute File Path to /root/basic and click Run.

Wait while the application is copied to the target using SSH protocols.

@imageSize{eclipse_run_wait.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_run_wait.png "Coping application to target"
@image latex eclipse/eclipse_run_wait.png "Coping application to target"

Finally, in the Console, see the traces and EGT application running on the target
board.

@section eclipse_debug Debug EGT Application Remotely

As a preliminary step, you also need to ensure that gdbserver is
present on your target. If gdbserver not present, then enable following
options in Buildroot menuconfig:

1. Toolchain -> GDB debugger Version
2. Toolchain -> Build cross gdb for the host
3. Target packages -> Debugging, profiling and benchmark -> gdb
4. Target packages -> Debugging, profiling and benchmark -> gdbserver
5. Target packages -> Debugging, profiling and benchmark -> full debugger

To get a nice debugging experience, you may want Buildroot to build
the libraries with debugging symbols. To do so, enable the
Build options -> build packages with debugging symbols option.

Create remote debug configuration

The create remote debug configuration will connect to the target board and
it will automatically download the compiled binary, start a gdb-server session and
connect to it from within the Eclipse IDE.

Create a remote debug configuration by right clicking on Project and select "Debug As" -> "Debug Configurations…"

@imageSize{eclipse_debug_configuration.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_debug_configuration.png "Select Debug Configurations"
@image latex eclipse/eclipse_debug_configuration.png "Select Debug Configurations"

In the Debug Configurations window, double Click "C/C++ Remote Application".

@imageSize{eclipse_debug_remote_app.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_debug_remote_app.png "Select C/C++ Remote Application"
@image latex eclipse/eclipse_debug_remote_app.png "Select C/C++ Remote Application"

To set up the remote debugging connection, select New under Connection and Choose
a connection type of SSH and configure the New Connection as follows:

@imageSize{eclipse_run_conn_finish.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_run_conn_finish.png "setup SSH Connection"
@image latex eclipse/eclipse_run_conn_finish.png "setup SSH Connection"

Set the Remote Absolute File Path to /root/basic.

To configure the gdb connection over ssh, go to the Debugger tab and in it you will see that
the GDB debugger: gdb setting currently points to the default debugger
on the host system. This is a x86_64 debugger and it does not understand the symbols and
messages from the gdb-server on the target. You must change this to point to the ARM-specific
gdb that is created by Buildroot. Change it to:

\<buildroot\>/output/host/bin/arm-buildroot-linux-gnueabi-gdb

To prevent some spurious SIGNALS and to help the GDB debugger, we have provided an
initialization script. Create a gdbinit file in your project folder and copy the below
line and set the gdbinit file path to the GDB command file:

@code{.sh}
handle SIGILL nostop
set sysroot buildroot/output/target
@endcode

@imageSize{eclipse_debug_debugger_tab.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_debug_debugger_tab.png "setup GDB Debugger"
@image latex eclipse/eclipse_debug_debugger_tab.png "setup GDB Debugger"

click on the Apply and Debug button.

Now in Eclipse you should be able to select the new Debug Configuration.
If a pop up appears, click on Yes in the confirm perspective switch window.
If you get an authentication Message dialog box, select Yes to accept it or else
Eclipse will change to the debug perspective and the application will
halt at main(). Press Resume (F8) to continue execution and the application should
continue to run on the target. You can view variables and disassembly code.

Enjoying the enhanced debug experience.

@imageSize{eclipse_debug_window.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_debug_window.png "Eclipse debug window"
@image latex eclipse/eclipse_debug_window.png "Eclipse debug window"

@section eclipse_host_libegt Build libegt on Host Machine

This sections describes how to build a standalone EGT library and its examples
on the host machine in Eclipse using Autotools. To fetch the Source from the Git repository,
refer to section @ref started_fetching.

Create a new CPP Project using File-> New -> Project. In the New Project window,
expand C/C++ and select C/C++ Project, then click Next.

@imageSize{eclipse_new_cpp_project.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_new_cpp_project.png "Create new cpp project"
@image latex eclipse/eclipse_new_cpp_project.png "Create new cpp project"

In the New C/C++ Project window, select C++ Managed Builds and click Next.

@imageSize{eclipse_new_cpp_project_template.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_new_cpp_project_template.png "Select C++ Managed Builds"
@image latex eclipse/eclipse_new_cpp_project_template.png "Select C++ Managed Builds"

In C++ Project window,

1. Set the project name.
2. Uncheck default location.
3. Browse to git clone location of a EGT directory.
4. Select "GNU Autotools Empty project" type.
5. Select "GNU Autotools Toolchain" and Click on the Finish button.

@imageSize{eclipse_host_create_cpp_project.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_host_create_cpp_project.png  "Eclipse create new cpp project"
@image latex eclipse/eclipse_host_create_cpp_project.png "Eclipse create new cpp project"

Now we can see in the console Tab:

1. The autoreconf command finished successfully and generated configure and Makefile.in files in the Project folder.
2. Executed configure scripts and generated Makefiles in the project folder.

@imageSize{eclipse_host_reconfigure_done.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_host_reconfigure_done.png  "Eclipse reconfigure done"
@image latex eclipse/eclipse_host_reconfigure_done.png "Eclipse reconfigure done"

To build the libegt project, press (Ctrl-b) or select the Project->"build all" option and observe build messages in Console.

@imageSize{eclipse_host_buildall.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_host_buildall.png  "Eclipse build libegt project"
@image latex eclipse/eclipse_host_buildall.png "Eclipse build libegt project"

@section eclipse_host_execute Execute EGT application on Host Machine.

Before we start to execute/debug an application, we need to set the environment variable LD_LIBRARY_PATH
to point to the libegt.so folder (i.e. src/.libs/).

Go to Project properties by right clicking the project and selecting Properties or Press (Alt + Enter).
In the Project properties window, Expand "C/C++ Build" and Select "Environment" and then click ADD.

@imageSize{eclipse_host_env.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_host_env.png  "Add Project Environment Variables"
@image latex eclipse/eclipse_host_env.png "Add Project Environment Variables"

In a New Environment variable window, set the below details and click OK and the OK again to close
the Project properties window.

1. Name: LD_LIBRARY_PATH
2. PATH: project/src/.libs/:$LD_LIBRARY_PATH.
3. Check Add to all Configurations.

@imageSize{eclipse_host_set_env.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_host_set_env.png "Eclipse set environment variable"
@image latex eclipse/eclipse_host_set_env.png "Eclipse set environment variable"

We can start executing the EGT example application by right clicking on the libegt Project
and selecting Run As -> "2.Local C/C++ Application".

@imageSize{eclipse_host_Run_as.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_host_Run_as.png "Select Local C/C++ Application"
@image latex eclipse/eclipse_host_Run_as.png "Select Local C/C++ Application"

In the "C Local Application" window, select basic under Binaries and click OK.

@imageSize{eclipse_host_select_application.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_host_select_application.png "Select basic Application under Binaries"
@image latex eclipse/eclipse_host_select_application.png "Select basic Application under Binaries"

A new EGT window will pop up running the EGT Basic example application.

@imageSize{eclipse_host_basic_example.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_host_basic_example.png "Eclipse basic example demo"
@image latex eclipse/eclipse_host_basic_example.png "Eclipse basic example demo"

@section eclipse_host_debug  Debug EGT application on Host Machine

To debug EGT example applications, right click on the libegt project
and select "Debug As" -> "2.Local C/C++ Application".

@imageSize{eclipse_host_debug_as.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_host_debug_as.png "Select Local C/C++ Application"
@image latex eclipse/eclipse_host_debug_as.png "Select Local C/C++ Application"

In the "C Local Application" window, select basic under Binaries and click OK.

@imageSize{eclipse_host_select_application.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_host_select_application.png "Select basic Application under Binaries"
@image latex eclipse/eclipse_host_select_application.png "Select basic Application under Binaries"


Eclipse will automatically switch to the Debug perspective window and the debugger stops
on the main function of the application and you can see disassembly code. Press
Resume (F8) to continue execution and the application should continue to run on the Host
Machine.

@imageSize{eclipse_host_debug_window.png,max-width:90%;height:auto;}
@image html eclipse/eclipse_host_debug_window.png "Eclipse debug window"
@image latex eclipse/eclipse_host_debug_window.png "Eclipse debug window"
