define({"283":{i:0.00421951197320902,u:"../content/processing/create_seismic_output/bayesian_classification.htm",a:"Bayesian classifications are used to link several attributes based on one or several  Probability Density Functions . First, one or several PDF(s) need to be provided. More will allow to select more PDFs. After clicking on Next \u003e\u003e, the PDF(s) can then optionally be normalized based on a priori ...",t:"6.1.4 Bayesian Classification"},"284":{i:0.00115179964307543,u:"../content/processing/create_seismic_output/create_from_wells.htm",a:"The Create seismic output from wells option writes loaded logs as seismic volumes. The dialog is accessed from the  Processing menu in the main toolbar; choose the  Create from Wells.. option. Select the well(s) in the left and the log(s) in the right list, note that several wells and logs can be ...",t:"6.1.5 Create from Well Logs"},"285":{i:0.00357364612724263,u:"../content/processing/create_seismic_output/pre-stack_processing.htm",a:"Prestack processing can be applied using different methods. It is the only place where prestack data can be output in OpendTect based on another prestack datastore. Open the Pre Stack processing window:  Processing \u003e Create Seismic Output \u003e Pre-Stack Processing The processing can be done by a number ...",t:"6.1.6 Pre-Stack Processing"},"286":{i:0.00213083643225928,u:"../content/processing/create_seismic_output/pre-stack_processing/mute.htm",a:"6.1.6.1 Mute Mute functions may be applied to Prestack gathers. This window will allow you to choose the mute definition, as well as to specify settings such as: Mute type: Outter (top) or Inner (tail) Taper length (in samples)",t:"6.1.6.1 Mute"},"287":{i:0.00115179964307543,u:"../content/processing/create_seismic_output/pre-stack_processing/automatic_gain_control.htm",a:"Automatic Gain Control (AGC) is one of the processing methods available for Prestack gathers. It will adjust the amplitude level using a sliding window of user-defined size (window width). Optionally, part of the lowest energy may be discarded from the amplitude level computation (in percent of the ...",t:"6.1.6.2 Automatic Gain Control"},"288":{i:0.00115179964307543,u:"../content/processing/create_seismic_output/pre-stack_processing/super_gather.htm",a:"A Super Gather may be used to laterally stack the traces in order to increase signal-to-noise ratio of Prestack gathers. The stack is controlled by an inline/crossline stepout, and the Shape (Cross or Rectangle). The computation is similar to a (non-steered)  volume statistics  attribute with a zero ...",t:"6.1.6.3 Super Gather"},"289":{i:0.00115179964307543,u:"../content/processing/create_seismic_output/pre-stack_processing/angle_mute.htm",a:"This processing method computes and applies a mute function. See the  processing method documentation . The only difference is that this method reads the offset range from the input prestack datastore, such that there is need to specify it. See  here  where and when the input velocity source needs ...",t:"6.1.6.4 Angle Mute"},"290":{i:0.00115179964307543,u:"../content/processing/create_seismic_output/seg-y_scanned_re-sort.htm",a:"The SEG-Y Scanned re-sort uses a scanned SEG-Y file and outputs it as a new file and re-writes the file-header. This tool is useful in case information in the header is poor or poorly sorted. In the Type field select the type of volume, either Pre-Stack or 3D volume. Next, select the scanned input ...",t:"6.1.7 SEG-Y Scanned Re-Sort"},"291":{i:0.00115179964307543,u:"../content/processing/create_seismic_output/velocity.htm",a:"6.1.8 Velocity Under \u0027velocity\u0027 sit two velocity-based conversion options:",t:"6.1.8 Velocity"},"292":{i:0.0042647742871723,u:"../content/processing/create_seismic_output/velocity/time-depth_conversion.htm",a:"To create an time-depth converted output, follow:  Processing \u003e Create Seismic Output \u003e Time-Depth Conversion... Time-depth conversion is done by applying Dix\u0027s equation, based on a interval- or RMS velocity volume. A  velocity model  and input time volume must be provided, and the direction of the ...",t:"6.1.8.1 Time-Depth Conversion"},"293":{i:0.0042647742871723,u:"../content/processing/create_seismic_output/velocity/velocity_conversion.htm",a:"This tool is started from  Processing \u003e Create Seismic Output \u003e Velocity conversion. It can be used to convert interval  velocity  volumes to RMS velocity volumes and vice versa. The conversion is applied using Dix\u0027s formula. Please note that for this reason it can only be applied in the time ...",t:"6.1.8.2 Velocity Conversion"},"294":{i:0.0108658336566816,u:"../content/processing/create_seismic_output/volume_builder_output.htm",a:"Volume-builder creator window can be launched from  Processing \u003e Create Seismic Output \u003e Volume Builder. It is used to create the output volume that has been defined in a  volume builder setup . Optionally, if the initial volume builder setup is not defined, press the  Edit button to define the ...",t:"6.1.9 Volume Builder Output"},"295":{i:0.00163397563270707,u:"../content/processing/create_horizon_output.htm",a:"6.2 Create Horizon Output This menu is used to create 2D/3D-grid based output. The data is stored as horizon data (or attribute) to the selected horizon.",t:"6.2 Create Horizon Output"},"296":{i:0.00115179964307543,u:"../content/processing/create_horizon_ouptut/attribute.htm",a:"Some attributes consume significant calculation time e.g. curvature, spectral decomposition etc. It also depends on the size of the input seismic volume. Therefore, to create on-the-fly a horizon data in a scene may take significant time. Following  Processing \u003e Create Horizon output \u003e Attribute (2D ...",t:"6.2.1 Attribute"},"297":{i:0.00115179964307543,u:"../content/processing/create_horizon_ouptut/stratal_amplitude.htm",a:"Stratal Amplitude is a processing tool available to compute statistics (min, max, rms, etc.) from an attribute along a horizon or between two horizons. The window can be launched from  Processing \u003e Create Horizon output \u003e Stratal Amplitude. The output will be stored as horizon data (grid) saved on ...",t:"6.2.2 Stratal Amplitude"},"298":{i:0.00115179964307543,u:"../content/processing/create_horizon_ouptut/isopach.htm",a:"In OpendTect isopach maps can be quickly calculated. The Create isopach window is launched either from the  Processing \u003e Create Horizon Output \u003e Isopach or from the right click menu of any horizon loaded in the tree: Workflows \u003e Create Isopach. In this window, select two horizons between which the ...",t:"6.2.3 Isopach"},"299":{i:0.00408891001062697,u:"../content/processing/start_batch_job.htm",a:"Batch jobs in OpendTect are stored under a job name in a file containing the inputs, parameters, log file and other relevant information. This information can be read by clicking on the \u0027Information\u0027 icon,  : Jobs may also be removed using the   icon. Batch  Execution: The batch job may be executed ...",t:"6.3 (Re-)Start Batch Job"},"300":{i:0.00133292219099163,u:"../content/processing/gmt.htm",a:"GMT (Generic Mapping Tools) is an open source collection of more than 60 tools for manipulating Geographic and Cartesian data sets. It can produce Encapsulated Postscript File illustrations ranging from simple x-y plots, via contour maps, to artificially illuminated surfaces and 3D perspectives ...",t:"6.4 GMT"},"301":{i:0.00115179964307543,u:"../content/processing/gmt/initial_setup.htm",a:"To launch  GMT tools, click on the icon   in the OpendTect main toolbar. The first time you launch the GMT mapping tools, a warning message will pop up, if GMT is not already installed on your computer. This can be downloaded from the  GMT website . After successful installation of the package, the ...",t:"6.4.1 Initial Setup"},"302":{i:0.00115179964307543,u:"../content/processing/gmt/create_postscript_maps.htm",a:"Several tabs have been arranged to specify the respective settings. The later part of this section shows a typical OpendTect example of a postscript map. Basemap: This tab is used to set the scale of the map and other map settings. Contours: It is used to create a horizon contour map. Faults: It is ...",t:"6.4.2 Create Postscript Maps"},"303":{i:0.00115179964307543,u:"../content/processing/madagascar.htm",a:"A generic user interface exists to  Madagascar , an open source seismic processing package that is very popular in seismic R\u0026D communities. In the builder, seismic pre- and poststack input and output files are either OpendTect or Madagascar formatted. The processing flow is constructed as a sequence ...",t:"6.5 Madagascar"},"304":{i:0.00115179964307543,u:"../content/processing/madagascar/madagascar_installation.htm",a:"Madagascar is an open-source, standalone software. To be used with OpendTect, Madagascar must first be installed, otherwise, when starting Madagscar , the next window will display an error message and missing program boxes. The Madagascar package needs to be installed (see  install ) and the RSFROOT ...",t:"6.5.1 Madagascar Installation"},"305":{i:0.00115179964307543,u:"../content/processing/madagascar/madagascar_processing_window.htm",a:"The Madagascar processing window can be launched from the OpendTect toolbar by pressing the Madagascar   icon. Select the input cube to be processed, and then choose a program or combination of programs. Programs are organized into groups of programs. Once one program is selected, a description of ...",t:"6.5.2 Madagascar Processing Window"},"306":{i:0.00115179964307543,u:"../content/processing/madagascar/toolbar.htm",a:"6.5.3 Toolbar The toolbar is composed of the file option and three shortcut items. The file option is as follows:  The toolbar contains three shortcuts to create, open, and save the flow:  This creates a new processing flow.  This will open a saved flow. This will save a newly created flow.",t:"6.5.3 Toolbar"},"307":{i:0.00115179964307543,u:"../content/processing/madagascar/processing_input.htm",a:"6.5.4 Processing Input The first step is to select an input cube. The input can be a 3D volume, a Prestack volume, a Madagascar volume, or None. It is possible to choose a volume sub-selection.",t:"6.5.4 Processing Input"},"308":{i:0.00115179964307543,u:"../content/processing/madagascar/madagascar_processing_output.htm",a:"6.5.5 Madagascar Processing Output The final step is to chose an output volume type. Similar to the input selection, it can be a 3D volume, a Prestack volume, a Madagascar volume, or None.",t:"6.5.5 Madagascar Processing Output"},"309":{i:0.00232528793245177,u:"../content/processing/batch_processing.htm",a:"6.6 Batch Processing Though Batch Processing does not actually appear in the pop-out menu under Processing, this is a convenient place in this User Documentation to present the information on these processes. ",t:"6.6 Batch Processing"},"310":{i:0.00161548255070756,u:"../content/processing/batch_processing/single_machine_batch_processing_window.htm",a:"In single mode processing, the data can be processed either on a local machine or on a remote host. All relevant information on the progress of the calculation will be stored, (see  Job information file ). \u0027Execute remote\u0027 toggled on to send the job to a remote machine. The job priority can be ...",t:"6.6.1 Single Machine Batch Processing Window"},"311":{i:0.00489527805198899,u:"../content/processing/batch_processing/multi-machine_batch_processing_window.htm",a:"The multi-machine batch processing window controls on which machines a volume output or SteeringCube batch job will be processed. Jobs are distributed over the Used hosts on a line-by-line basis (the number of inlines per job can be specified). Hosts can be Added and/or Stopped at all times. ...",t:"6.6.2 Multi-Machine Batch Processing Window"},"312":{i:0.00252497045800122,u:"../content/processing/batch_processing/batch_log_file.htm",a:"A batch log file is produced for every volume output run. The information is streamed to a file if the batch job is executed on a remote computer. If the processing is done locally the log file is either streamed to a new, dedicated window, or to the standard output window. Every N traces the ...",t:"6.6.3 Batch Log File"},"313":{i:0.00232528793245177,u:"../content/processing/batch_processing/cluster_processing.htm",a:"Batch jobs can be run from OpendTect to cluster management tools. So far dGB has successfully tested  SLURM  which is easy to install and even easier to use. Cluster processing is enabled by following this prerequisites: The environment variable DTECT_CLUSTER_PROC is set to yes before starting ...",t:"6.6.4 Cluster Processing"},"314":{i:0.00115179964307543,u:"../content/scenes.htm",a:"The OpendTect main window can have multiple scenes, most of them opened using this menu. The scenes behave like sub-windows within the main window: Each scene has its own tree and can be minimized, maximized, reduced or enlarged in size, without ever going out of the main window. The trees of ...",t:"7 Scenes"},"315":{i:0.00650543268481544,u:"../content/scenes/time_and_depth-converted_scenes.htm",a:"OpendTect can display time data in the depth domain and depth data in the time domain. This is done using a  user-selected velocity volume  and computing the new Z range (depth or time) based on the original Z range (time or depth respectively). In all transformed scenes each and every display ...",t:"7.1 Time- and Depth-Converted Scenes"},"316":{i:0.0024140845505313,u:"../content/scenes/flattened_horizon_scenes.htm",a:"7.2 Flattened Horizon Scenes This option will generate a new scene flattened about the selected horizon. The Z range has the same unit as the original scene, but it is now relative to that horizon and no longer absolute.",t:"7.2 Flattened Horizon Scenes"},"317":{i:0.00143504776134745,u:"../content/scenes/wheeler_scenes.htm",a:"The Wheeler Scene is a transformation (flattening) of  HorizonCube  into relative geological time (RGT). Therefore, before adding a Wheeler Scene, the HorizonCube will need to be selected. You will be prompted for this if not already selected.   This option is only available if the  SSIS plugin  is ...",t:"7.3 Wheeler Scenes"},"318":{i:0.00115179964307543,u:"../content/view.htm",a:"8 View",t:"8 View"},"319":{i:0.00115179964307543,u:"../content/view/work_area.htm",a:"The Work area dialog is opened from the \u0027view\u0027 menu. The Work area sets the area bounded by the survey box. Displayed items will be cropped automatically to fit the set inline, cross-line and z-ranges.  Set ranges to full survey – to fully maximize the inline, crossline and Z  Open subselection – ...",t:"8.1 Work Area"},"320":{i:0.00115179964307543,u:"../content/view/z-scale.htm",a:"The Z-scale option allows scaling of the survey box vertically. There are three options for Z-scaling: The slice bar, setting scaling according to current scene \"Fit to scene\" and resetting the scaling \"To Home\". You can set any position/orientation of the scene by clicking on  as default Home.",t:"8.2 Z-Scale"},"321":{i:0.00115179964307543,u:"../content/view/stereo_viewing.htm",a:"The Stereo viewing menu allows setting the stereo viewing on/off. Note that in order to use Red/Cyan stereo, appropriate glasses are needed. The offset between the red and cyan view can be manipulated with the Stereo Offset menu. The Quad buffer option has special hardware requirements in order to ...",t:"8.3 Stereo Viewing"},"322":{i:0.00115179964307543,u:"../content/view/toolbars.htm",a:"8.4 Toolbars All elements available in the main OpendTect window can be switched on/off here. See  Toolbars  for the various actions of the buttons on the toolbars.",t:"8.4 Toolbars"},"323":{i:0.00115179964307543,u:"../content/utilities.htm",a:"9 Utilities",t:"9 Utilities"},"324":{i:0.00115179964307543,u:"../content/utilities/settings.htm",a:"9.1 Settings The settings for Fonts, Mouse, Keyboard, etc., can be changed from  Utilities--\u003e Settings--\u003e",t:"9.1 Settings"},"325":{i:0.00213083643225928,u:"../content/utilities/settings/look_and_feel.htm",a:"This option brings up an interface containing several tabs for defining various settings in OpendTect: These are explained in detail below: General  Default Icon Size is 32. For systems with smaller screens (esp. laptops) it may be useful to reduce this value to 28 or even 24. In combination with ...",t:"9.1.1 Look and feel"},"326":{i:0.00115179964307543,u:"../content/utilities/settings/mouse_controls.htm",a:"9.1.2 Mouse Controls The mouse buttons can be set differently. System administrators can implement user-defined mouse button actions. See the Application Management Documentation for more details.",t:"9.1.2 Mouse Controls"},"327":{i:0.00115179964307543,u:"../content/utilities/settings/keyboard_shortcuts.htm",a:"9.1.3 Keyboard Shortcuts The user can define his/her own keyboard shortcuts to move a slice forward/backward. The user can use one key (set the first key to no-button) or a combination of control or shift key, plus another key which can be selected from a long list.",t:"9.1.3 Keyboard Shortcuts"},"328":{i:0.00115179964307543,u:"../content/utilities/settings/advanced_settings.htm",a:"The ADVANCED  user settings are used to change the default settings of specific keywords. The user can specify his/her own settings and/or setup or edit the Survey defaults settings. These settings can be found in the .od/ directory from the user home directory. Depending on the platform used, this ...",t:"9.1.4 Advanced Settings"},"329":{i:0.00213083643225928,u:"../content/utilities/settings/advanced_settings/personal_settings.htm",a:"A user can update the Personal Settings by specifying a keyward (a variable name) and its value. The value column becomes editable if left mouse button is double clicked in a cell. A brief description on the available variables (and their values) is given below: Most of the changes are updated after ...",t:"9.1.4.1 Personal Settings"},"330":{i:0.00115179964307543,u:"../content/utilities/settings/advanced_settings/survey_defaults.htm",a:"This option enables the user to setup/edit the Survey default settings. The value can be changed by double clicking in a cell. Depending on the user, several settings are available. Let us describe few: 3DAttrset.Auto ID: This setting shows the ID value of the default auto-load attribute set ...",t:"9.1.4.2 Survey Defaults"},"331":{i:0.00115179964307543,u:"../content/utilities/tools.htm",a:"9.2 Tools",t:"9.2 Tools"},"332":{i:0.00213083643225928,u:"../content/utilities/tools/batch_programs.htm",a:"To run the batch program go to:  Utilities \u003e Tools \u003e Batch programs Choose the batch program you need to run, the available are: cbvs_browse, cbvs_dump, lmhostid, glxinfo, ivfileviewer. The text box will show comments and details. If another OpendTect batch program is chosen, fill in the required ...",t:"9.2.1 Batch Programs"},"333":{i:0.00115179964307543,u:"../content/utilities/tools/position_conversion.htm",a:"Position Conversion is an utility that can be used to convert the position pairs from Inline/Crossline to X/Y, and vice versa. This utility can be launched either directly from Utility \u003e Position Conversion ...  or from Survey selection menu ( Survey \u003e Select/Setup). In the position conversion ...",t:"9.2.2 Position Conversion"},"334":{i:0.00115179964307543,u:"../content/utilities/tools/create_plugin_development_environment.htm",a:"With this option the user can create the OpendTect developer environment to develop a plugin to OpendTect. The source code and all other relevant files are copied into a user specified directory (chosen from the Package Manager during the installation setup). More information can be found in the ...",t:"9.2.3 Create Plugin Development Environment"},"335":{i:0.00115179964307543,u:"../content/utilities/tools/command_driver.htm",a:"The Command Driver offers automated control of the current OpendTect application from a command script. The command script is a replacement for a series of keyboard and mouse interactions performed by the user. It can be used to automate parts of the workflow, and helps to speed up executing ...",t:"9.2.4 Command Driver"},"336":{i:0.00115179964307543,u:"../content/utilities/tools/presentation_maker_plugin.htm",a:"Introduction Python-pptx is a Python library for creating and updating PowerPoint (.pptx files). The OpendTect \u0027Presentation Maker\u0027 plugin uses this library to create a PowerPoint presentation from scene, window or desktop screenshots. Python-pptx installation General installation information is at ...",t:"9.2.5 Presentation Maker Plugin"},"337":{i:0.00115179964307543,u:"../content/utilities/installation.htm",a:"9.3 Installation",t:"9.3 Installation"},"338":{i:0.0029630251222374,u:"../content/utilities/installation/update.htm",a:"Some improvements in the installation manager: Removing individual packages is now supported Windows program feature to update or uninstall OpendTect Improvements to proxy handling The Installation Manager is available for download via the appropriate platform link on the  download page  of the ...",t:"9.3.1 Update (Installation Manager)"},"339":{i:0.00115179964307543,u:"../content/utilities/installation/update/package_manager.htm",a:"The last window of the wizard is the OpendTect Package Manager (see above figure). Multiple items can be selected from the list by checking the boxes (or not). Optionally, the relevant package combination could also be selected from the top list box. The installation manager will automatically ...",t:"9.3.1.1 Package Manager"},"340":{i:0.00115179964307543,u:"../content/utilities/installation/update/package_manager/utilities_menu.htm",a:"9.3.1.1.1 Utilities Menu On the top left corner of the package selection window there is a Utilities menu, which offers some useful functions for the installation manager:",t:"9.3.1.1.1 Utilities Menu"},"341":{i:0.00115179964307543,u:"../content/utilities/installation/update/package_manager/utilities_menu/export_download_list.htm",a:"This option allows the user to download the list of URLs of the individual packages from the download site. This list is stored in a text file which can be used later to download these files directly without the help of the installer program. After downloading, user can run his/her own unzipping ...",t:"9.3.1.1.1.1 Export Download List"},"342":{i:0.00115179964307543,u:"../content/utilities/installation/update/package_manager/utilities_menu/rollback.htm",a:"Rollback tool allows you to restore your previous version of the installation. If after updating the software you feel uncomfortable with some of the new features and want to go back to your previous installation, you have to use this tool. As this tool will change your entire installation so you ...",t:"9.3.1.1.1.2 Rollback"},"343":{i:0.00115179964307543,u:"../content/utilities/installation/update/package_manager/utilities_menu/show_log_file.htm",a:"The installation manager keeps track of all the action it is executing in a log file. This log file can be viewed from this tool. This is useful for debugging purposes. If you face any trouble during the installation process you can send this file to OpendTect support if needed.",t:"9.3.1.1.1.3 Show Log File"},"344":{i:0.00367039327130167,u:"../content/utilities/installation/update/package_manager/offline_installation.htm",a:"You may also choose to create packages for offline installation. These packages are created in such a way as to function cross-platform. For example, you may download the Linux 64bit package onto a Windows machine and then transfer and install it onto the Linux system or vice-versa. You will need to ...",t:"9.3.1.1.2 Offline Installation"},"345":{i:0.00115179964307543,u:"../content/utilities/installation/update/package_manager/offline_installation/package_verification.htm",a:"We generate signature files for all packages. Normally, a package is a zip file downloadable from our website: For OpendTect version 4.6, please visit:  OD version 4.6  page. For OpendTect version 5.0, please visit:  OD version 5.0  page. For each zip-file, there is a zip.sig file containing a ...",t:"9.3.1.1.2.1 Package Verification"},"346":{i:0.00115179964307543,u:"../content/utilities/installation/auto-update_policy.htm",a:"The auto-update policy can be defined and changed by a user. By default the option is set to [Inform] when the updates are available. On Windows, this can be changed to [None] Never check for updates should you prefer. On Linux, there are two additional options - [Manager] and [Auto]:",t:"9.3.2 Auto-Update Policy"},"347":{i:0.00213083643225928,u:"../content/utilities/installation/connection_settings.htm",a:"To enter the proxy information, the correct proxy server information must be added in the Connection Settings before running the installation. This is done in the following dialog. This dialog is also available directly through the  Installation Manager  on clicking the Proxy Settings button.",t:"9.3.3 Connection Settings"},"348":{i:0.00115179964307543,u:"../content/utilities/installation/plugins.htm",a:"The plugins window lists the plugins that are currently loaded (or not) into OpendTect, and provides relevant license information. Developers might want to use the option \"Load a plugin\" to manually load their plugin. The developers documentation describes how to add a plugin to the automatic ...",t:"9.3.4 Plugins"},"349":{i:0.00531282585717652,u:"../content/utilities/installation/setup_batch_processing.htm",a:"In order to utilize OpendTect\u0027s capability for Multi-Machine Processing (MMP), a BatchHosts file must be created and used. This file  contains the list of remote machines (host machines or nodes ) and some relevant details about these machines and the path to the Survey Data Root.  OpendTect will ...",t:"9.3.5 Setup Batch Processing"},"350":{i:0.00115179964307543,u:"../content/utilities/installation/licenses.htm",a:"Under  Utilities--\u003e Installation--\u003e Licenses you will see two sets of options, differing per platform:  License options under Windows License options under Linux For information about floating or server-based licenses, please refer to the  flexnet installation guide  page For more general ...",t:"9.3.6 Licenses"},"351":{i:0.00115179964307543,u:"../content/utilities/installation/licenses/install_demo_node-locked_license.htm",a:"Plugins to OpendTect can be run either by using a license server or by using demo (evaluation) licenses. This second case is case called \"node-locked license installation\".  Use the following window to specify the path to the node-locked (demo/evaluation) license files that were given to you: Here ...",t:"9.3.6.1 Install demo/node-locked license"},"352":{i:0.00115179964307543,u:"../content/utilities/installation/licenses/clear_license_installation.htm",a:"This option (Windows only) will clear: Demo or node-locked licenses installed via any route, including the \u0027Install demo license\u0027 option. Floating (or \u0027server\u0027) licenses that may have been installed (without stopping the license server). Once cleared, you will be prompted to restart: Users of Linux ...",t:"9.3.6.2 Clear License Installation"},"353":{i:0.00115179964307543,u:"../content/utilities/installation/licenses/show_hostid.htm",a:"Clicking this option  will pop up a simple dialog showing the HostID of the machine: Additionally, on Windows, accessing the HostID of the machine can be done via the LM Tools (available via the Start Menu or directly from ..\\OpendTect\\5.0.0\\bin\\win64\\lm.dgb\\lmtools.exe): The option \u0027Save ...",t:"9.3.6.3 Show HostID"},"354":{i:0.00115179964307543,u:"../content/utilities/show_log_file.htm",a:"9.4 Show Log File The user can check the log file from  Utilities \u003e Show log file. This will show the log of low traffic signals e.g. warning messages if a plugin (or license) is not properly loaded.",t:"9.4 Show Log File"},"355":{i:0.00115179964307543,u:"../content/help.htm",a:"10 Help From this drop-down menu, some of the various help aids can be accessed. This includes the  OpendTect User Documentation  (F1) and the  dGB Plugins User Documentation  (F2). About gives you some basic information, as seen here:",t:"10 Help"},"356":{i:0.00271078274079643,u:"../content/appendix_a-attributes_and_filters.htm",a:"OpendTect attributes and filters are divided into \u0027dip-steering attributes\u0027 and \u0027no-steering attributes\u0027. A comprehensive list of all the attributes and filters available in the free, open source OpendTect core software is shown below (the so-called \u0027no-steering attributes\u0027): Note that other ...",t:"11 Appendix A - Attributes and Filters"},"357":{i:0.00124042213626033,u:"../content/appendix_a-attributes_and_filters/ceemd-spectral_decomposition.htm",a:"Name CEEMD (Complete Ensemble EMD) – Group of Spectral Decomposition algorithms which are based on Empirical Mode Decomposition (Huang et al., 1998). Description The Empirical Mode Decomposition (EMD) algorithms implemented in OpendTect follow the work published by Jiajun Han and Mirko van der Baan ...",t:"11.1 CEEMD - Spectral Decomposition"},"358":{i:0.0030516476154223,u:"../content/appendix_a-attributes_and_filters/convolve.htm",a:"Name Convolve (2D/3D) -- Attribute that returns a filtered response. Description The input data is convolved with a three-dimensional kernel specified by Filter type and associated parameters. Lowpass, Laplacian, and Prewitt are well known filters in image processing. Input Parameters Lowpass ...",t:"11.2 Convolve (2D \u0026 3D)"},});