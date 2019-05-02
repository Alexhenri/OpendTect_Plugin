define({"212":{i:0.00115179964307543,u:"../content/survey/pre-load/preload_horizons.htm",a:"For fast visualizations, multiple horizons (2D/3D) can also be loaded in to the memory. Thus whenever the preloaded horizons would be displayed in the scene, it will take less time in reading the file from the disk. Therefore, the functionality improves the visualization speed. Press the \u0027Add ..\u0027 ...",t:"4.6.2 Preload Horizons"},"213":{i:0.00115179964307543,u:"../content/survey/pre-load/preload_horizoncube.htm",a:"4.6.3 Preload HorizonCube For fast visualizations, HorizonCube (2D/3D) can also be loaded in to the memory. Thus whenever the preloaded HorizonCube would be displayed in the scene, it will take less time in reading the file from the disk.",t:"4.6.3 Preload HorizonCube"},"214":{i:0.00115179964307543,u:"../content/analysis.htm",a:"5 Analysis",t:"5 Analysis"},"215":{i:0.00115179964307543,u:"../content/analysis/attributes.htm",a:"In OpendTect, seismic attributes are calculated/evaluated by using Attribute-set Window. In this window, many single/multi trace, pre/poststack, dip-steered/non dip-steered attributes are available. Moreover, it also contains special filters (e.g. Gap decon, Frequency filters, dGB-special filters ...",t:"5.1 Attributes"},"216":{i:0.00272913669231607,u:"../content/analysis/attributes/attribute_set_window.htm",a:"The attribute set window contains a set of seismic attributes definitions to be evaluated/calculated. While defining the attributes it is possible to work in the active scene. Attributes can also be calculated after saving the attribute set. In broad sense following workflows are applicable in ...",t:"5.1.1 Attribute Set Window"},"217":{i:0.00173174595161258,u:"../content/analysis/attributes/attribute_set_toolbar.htm",a:"The attribute set toolbar comprises the following icons. Also accessible via the File menu, these are, from left to right: New set clears the window to create a new attribute set. The attribute set name can be specified when saving it (press OK, or select File - Save set menu option). Open set opens ...",t:"5.1.2 Attribute Set Toolbar"},"218":{i:0.00272758662642617,u:"../content/analysis/attributes/auto-load_attribute_set.htm",a:"By default, no attribute set is loaded at startup. These settings can be over-ruled by selecting a specific attribute set to be auto-loaded in the list each time the OpendTect window is started. This can be set from the attribute set window under File \u003e Auto Load Attribute Set sub-menu. If selected, ...",t:"5.1.3 Auto-Load Attribute Set"},"219":{i:0.00173174595161258,u:"../content/analysis/attributes/default_attribute_sets.htm",a:"OpendTect is provided with \"Default attribute sets\" to get you started. By selecting a default attribute set, a window appears to select the correct input volume(s) and the correct SteeringCube (see images below). These attributes (except \"Evaluate Attributes\") require the following dGB plugins: ...",t:"5.1.4 Default Attribute Sets"},"220":{i:0.00115179964307543,u:"../content/analysis/attributes/input_selection.htm",a:"5.1.5 Input Selection Every attribute requires input data. Both stored data and already defined attributes can be used as input to a new attribute. In other words, attributes can be embedded. However, circular references are not possible.",t:"5.1.5 Input Selection"},"221":{i:0.00115179964307543,u:"../content/analysis/attributes/input_selection/input_selection_for_3d_attribute_sets.htm",a:"Select from the stored data or from the list of defined attributes in the \"active\" attribute set. In case the input data is multi-component, it is possible to choose from the available components as shown below (or include ALL).  The Filter section allows to quickly find the right input. e.g type *S ...",t:"5.1.5.1 Input Selection for 3D Attribute Sets"},"222":{i:0.00115179964307543,u:"../content/analysis/attributes/input_selection/input_selection_for_2d_attribute_sets.htm",a:"5.1.5.2 Input Selection for 2D Attribute Sets Select from the stored data or from the list of already defined attributes in the current attribute set.  If the selected stored data set is multi-component, the user will get an option to choose which component to select as input data:",t:"5.1.5.2 Input Selection for 2D Attribute Sets"},"223":{i:0.0022212643462045,u:"../content/analysis/attributes/import_an_attribute_set_from.htm",a:"Attributes are primarily stored in attribute set files of extension .attr Attribute definitions can also be found in the parameter files of a processing job when an attribute was used to process a volume or data set. It is possible to import the attribute set of an attribute file from the menu: File ...",t:"5.1.6 Import an Attribute Set from"},"224":{i:0.00139655884037139,u:"../content/analysis/attributes/calculate_attributes.htm",a:"The attribute evaluation process has been considered critically and thus several key options are available for the user. For instance, Evaluate Attribute  is considered as intermediate (but not necessary) step to quickly analyze the different parameters of any attribute within the working ...",t:"5.1.7 Calculate Attributes"},"225":{i:0.011300882842114,u:"../content/analysis/volume_builder_setup.htm",a:"The volume builder   setup is used to apply volume-based operations, unlike the attributes that work trace-by-trace. The setup is launched via the  Analysis \u003e Volume Builder menu. The setup is a very useful tool for gridding velocities or other rock properties. The volume builder setup window ...",t:"5.2 Volume Builder Setup"},"226":{i:0.00115179964307543,u:"../content/analysis/volume_builder_setup/body_shape_painter.htm",a:"The body shape painter is used to define the inside and outside values for an OpendTect body. The options for \u0027Inside Fill Type\u0027  are shown in the image below. The same options are available for \u0027Outside Fill Type\u0027: Fill Types: Constant Value: Single, user-defined value: Previous step: takes the ...",t:"5.2.1 Body Shape Painter"},"227":{i:0.00115179964307543,u:"../content/analysis/volume_builder_setup/lateral_smoother.htm",a:"The lateral smoothing is a rectangular two-dimension smoothing filtering method of the volume. The average filtering will be done in the frequency domain by applying a 2D FFT to the Z slices. This requires a rectangular dataset, while the input can be irregular. The filtering type can be chosen ...",t:"5.2.2 Lateral Smoother"},"228":{i:0.00115179964307543,u:"../content/analysis/volume_builder_setup/smoother.htm",a:"The smoother step is used to apply a three dimensional smoothing operator by specifying In-line, Cross-line and Vertical (ms) stepouts for 3D or two dimensional smoothing with Trace and Vertical (ms) stepouts in case of 2D. Various operator shapes can be chosen (e.g. Hamming) and can be visualized ...",t:"5.2.3 Smoother"},"229":{i:0.00115179964307543,u:"../content/analysis/volume_builder_setup/horizon-based_painter.htm",a:"The horizon-based painter is used to create a model between two surfaces. The initial top and bottom values are necessary to be filled in the input. The intermediate values are interpolated to a survey or a horizon. In this window, horizons have to be selected as top/base values. The slope type is ...",t:"5.2.4 Horizon-Based Painter"},"230":{i:0.00619828620478642,u:"../content/analysis/volume_builder_setup/velocity_gridder.htm",a:"The gridding will create a volume out of a sparsely sampled dataset. The input source MUST be  tagged  with a velocity type. Indeed the gridding is applied to the time-depth relation hold by the velocity source and not on the amplitudes of the velocity source. This preserves the time-depth relation ...",t:"5.2.5 Velocity Gridder"},"231":{i:0.00115179964307543,u:"../content/analysis/volume_builder_setup/input_volume.htm",a:"5.2.6 Input Volume This step is in general used to provide a background volume or 2D data attribute before using spatially constrained steps, for instance before the body shape painter or horizon-based painter.",t:"5.2.6 Input Volume"},"232":{i:0.00115179964307543,u:"../content/analysis/volume_builder_setup/voxel_connectivity_filter.htm",a:"Voxel Connextivity Filter is a special tool to create continuous bodies based on the amplitudes in a stored volume. A \u0027 voxel \u0027 is defined as the volume around one sample. It is thus linked to the survey bin size and sampling rate.  This volume builder step must be preceded by a step providing the ...",t:"5.2.7 Voxel Connectivity Filter"},"233":{i:0.00115179964307543,u:"../content/analysis/volume_builder_setup/well_log_interpolator.htm",a:"This gridding step is used to populate a 3D volume using well logs by interpolating along Z-slices. Vertical Extension: Select the method of vertical extension from the following options: Log extension if needed: Extend the logs (if required) to match the Selected zone Extract Between: Extract data ...",t:"5.2.8 Well Log Interpolator"},"234":{i:0.00338930611367019,u:"../content/analysis/cross-plot.htm",a:"The Cross-plot tool is designed to create two dimensional cross plots between 2D/3D seismic data (attributes) and either other attributes or well data. The data can be analysed in multiple maners, using different kinds of colour coding and data selection tools. It may be launched from the menu  ...",t:"5.3 Cross-Plot"},"235":{i:0.00115179964307543,u:"../content/analysis/cross-plot/cross-plot_data_extraction.htm",a:"The crossplot data must first be extracted, either on (a subset of) the  horizon  or along (deviated)  well paths . 2D or 3D attributes can be used, and well logs if the extraction is done along the well paths. The extracted data will first be presented in a  table  before actually selecting the ...",t:"5.3.1 Cross-Plot Data Extraction"},"236":{i:0.00316579593931861,u:"../content/analysis/cross-plot/cross-plot_data_extraction/well-based_data_extraction.htm",a:"This window presents the attributes and/or logs that can be extracted along well path. The output will be presented in table before being used for cross-plot. At least one well must be selected, and one attribute or one log. It is also possible to select only attributes, or only logs. The well  ...",t:"5.3.1.1 Well-based Data Extraction"},"237":{i:0.0116977834476854,u:"../content/analysis/cross-plot/cross-plot_data_extraction/attribute-based_extraction_window.htm",a:"Attributes based data extraction window is used to extract an attribute data (stored volumes or a defined attribute) within a volume defined by a range, polygon, surfaces, body or a well path with lateral extension. The same window (shown below) could also be used to extract an attribute data along ...",t:"5.3.1.2 Attribute-based Extraction Window"},"238":{i:0.0119924231535373,u:"../content/analysis/cross-plot/cross-plot_data_extraction/location_filters.htm",a:"The filters should be used to further limit the amount of data to be extracted for making cross-plots. Several filters can be used simultaneously. Filters are of two types: The first two filters, Random and Subsample, are not position related. The Random filter passes a certain percentage of random ...",t:"5.3.1.3 Location Filters"},"239":{i:0.00575646195953459,u:"../content/analysis/cross-plot/cross-plot_table.htm",a:"The crossplot table displays the extracted data. It is used to edit and plot the data for a crossplot. A row in the table corresponds to one extracted data point, annotated by its position (X, Y and Z) and followed by the collected attributes values (forming at vector, from left to right: logs, ...",t:"5.3.2 Cross-Plot Table"},"240":{i:0.003027583080267,u:"../content/analysis/cross-plot/cross-plot_window.htm",a:"The cross-plot window shows the data previously extracted  and shown the cross-plot . The window may start empty if insufficient data was selected in the table window. The cross-plot window contain three toolbars to perform various tasks: Crossplot manipulation toolbar  Take the snapshot of the ...",t:"5.3.3 Cross-Plot Window"},"241":{i:0.00115179964307543,u:"../content/analysis/cross-plot/cross-plot_window/cross-plot_properties.htm",a:"The properties window (accessed via  )can be used to adjust the scale, view statistics, add regressing lines etc. Scaling Tab: Sets the clipping state for each axis, or the amplitude range for the chart. Default: 0, which means that the window is adjusted to fit the entire amplitude distribution. ...",t:"5.3.3.1 Cross-Plot Properties"},"242":{i:0.0029610422917981,u:"../content/analysis/cross-plot/cross-plot_window/probability_density_functions.htm",a:"Probability Density Functions (PDFs) can be created from the cross-plot toolbar icon  . This icon launches a pop-up dialog that can be used for selecting attributes in order to create PDFs. The PDFs are stored in OpendTect Format, that can later be used for running  Bayesian classifications . Please ...",t:"5.3.3.2 Probability Density Functions"},"243":{i:0.00115179964307543,u:"../content/analysis/cross-plot/cross-plot_window/overlay_from_a_third_attribute.htm",a:"5.3.3.3 Overlay from a Third Attribute Scattered points can be coded with respect to the amplitudes of an attribute using that option. The popup window requires the selection of that third attribute, and colorbar specifications (type and amplitude range)",t:"5.3.3.3 Overlay from a Third Attribute"},"244":{i:0.00115179964307543,u:"../content/analysis/cross-plot/cross-plot_window/well-based_color_coding.htm",a:"The scattered points in the cross-plot window can be coded with respect to the wells along which the data points were originally  extracted . The following utility window can be used to control the colour associated to each well: The configuration above gives the following result:",t:"5.3.3.4 Well-based Color Coding"},"245":{i:0.00115179964307543,u:"../content/analysis/cross-plot/cross-plot_window/selection_settings.htm",a:"The selections made interactively in the cross-plot window can be further refined and managed in this window. The \u0027Refine\u0027 option ( )utilizes mathematical logic to restrict the selection according to the range set in the Refine Selection window (a pop-up window invoked when this button is pressed). ...",t:"5.3.3.5 Selection Settings"},"246":{i:0.00278282388297104,u:"../content/analysis/cross-plot/open_cross-plot.htm",a:"5.3.4 Open Crossplot This option allows you to open previously saved or imported crossplot data and will directly open the data in the  crossplot table .",t:"5.3.4 Open Crossplot"},"247":{i:0.00115179964307543,u:"../content/analysis/wells.htm",a:"5.4 Wells The Wells  element in the Analysis menu gives you access to three features:  Edit logs... , Tie Well to Seismic...  and  Rock Physics...",t:"5.4 Wells"},"248":{i:0.00215956368376854,u:"../content/analysis/wells/well_log_tools.htm",a:"The well log tools can be used to remove spikes, smooth and clip the logs. it can also accessed by using the  icon in the  Manage Wells  window. Multiple wells can be selected at once along-with the various logs. The logs can be extracted between: Markers: The bounding markers have to be specified. ...",t:"5.4.1 Well Log Tools"},"249":{i:0.00263830457657009,u:"../content/analysis/wells/tie_well_to_seismic.htm",a:"Launch the Well-Seismic Tie window from the main menu or, optionally, the Well-Seismic Tie window can be launched from the tree. Well to the seismic tie is a major task for interpretation projects. It is used to correlate the well information (logs) to the 3D seismic volume or 2D seismic lines. This ...",t:"5.4.2 Tie Well to Seismic"},"250":{i:0.00115179964307543,u:"../content/analysis/wells/tie_well_to_seismic/well-tie_selection_window.htm",a:"The tie well to seismic window is used to select the necessary data for the Well-Seismic tie workflow. Please have a look at the introduction to see how to prepare the necessary data. Well tie can be used to tie the well with the 3D seismic volume or 2D seismic line. Well to 3D seismic tie window ...",t:"5.4.2.1 Well-Tie Selection Window"},"251":{i:0.00115179964307543,u:"../content/analysis/wells/tie_well_to_seismic/well-tie_checkshot_editor.htm",a:"In OpendTect, CheckShot corrections are applied before launching the Well to Seismic Tie window. If you have no depth/time model or have not selected any existing one, you will be proposed to correct the sonic integrated depth/time model, provided you imported a CheckShot model for your well. The ...",t:"5.4.2.2 Well-Tie Checkshot Editor"},"252":{i:0.00115179964307543,u:"../content/analysis/wells/tie_well_to_seismic/well-tie_display_panel.htm",a:"The display panel is the main window where the wells are tied to the seismic data. This module is primarly used to update the current (loaded) time-depth curve. Previous, intermediate and final TD curves can be exported to ascii at any moment using the following icon  . A secondary output of the ...",t:"5.4.2.3 Well-Tie Display Panel"},"253":{i:0.00115179964307543,u:"../content/analysis/wells/tie_well_to_seismic/well-tie_crosscheck_parameters.htm",a:"The cross-checking window is launched from the well ties display panel (Section-  Well Tie: Display Panel ) by clicking on the Display additional information button. The window contains the initial and estimated wavelet information. The wavelet can be computed between the two levels (start-end of ...",t:"5.4.2.4 Well-Tie Crosscheck Parameters"},"254":{i:0.0236957537139821,u:"../content/analysis/layer_modeling.htm",a:"Pseudo-wells are stratigraphic columns with attached well logs, but without geographical location. Any pseudo-well can be seen as a possible realization of a newly drilled well in the area. The pseudo-wells generation is achieved following a model that has to be defined. To achieve Layer Modeling, ...",t:"5.5 Layer Modeling"},"255":{i:0.00115179964307543,u:"../content/analysis/layer_modeling/basic_modeling.htm",a:"Each layer in the stratigraphy column is characterized by different rock properties. The model, based on the stratigraphy, is assigning properties to each lithology, layer by layer. The model is built using a blocky approach. The different properties are selected within a list. Their value can ...",t:"5.5.1 Basic"},"256":{i:0.0081055585093701,u:"../content/analysis/layer_modeling/layer_description.htm",a:"First of all, the Layer Succession has to be defined and will be used to create the pseudo-wells. Layer Modeling Window (in red : Layer Description elements) To start defining layers properties, the user has to click on \"click to add\" on the left rectangle. Once one layer has been defined, click on ...",t:"5.5.1.1 Layer Description"},"257":{i:0.00115179964307543,u:"../content/analysis/layer_modeling/synthetic_and_property-log_generation.htm",a:"The property logs of the pseudo-wells only need the  Layer Description  to be created. To generate synthetic traces for each pseudo-well, three elements have to be provided: To generate the synthetics: Wavelet To generate synthetics for a range of offset : Ray tracer parameters Layer properties: The ...",t:"5.5.1.2 Synthetic- and Property-Log Generation"},"258":{i:0.00115179964307543,u:"../content/analysis/layer_modeling/synthetic_and_property-log_generation/synthetic_layer_properties.htm",a:"The synthetic seismic generation requires different quantities : Density, P wave velocity and S wave velocity. These quantities can be specified in clicking on the icon   : they can be computed using formulas and the appropriate modeled quantities. If the quantity has been modeled, it can be used as ...",t:"5.5.1.2.1 Synthetic Layer Properties"},"259":{i:0.00213083643225928,u:"../content/analysis/layer_modeling/synthetic_and_property-log_generation/wavelet.htm",a:"The wavelet can be selected from the one already available in the project and listed or a new one can be created in the  Wavelet manager  accessible from the  icon  . Some workflows need to have the synthetic with the same amplitude that the real seismic. The purpose to the scaler is to scale the ...",t:"5.5.1.2.2 Wavelet"},"260":{i:0.00115179964307543,u:"../content/analysis/layer_modeling/synthetic_and_property-log_generation/ray_tracing.htm",a:"The ray tracer, available via the   icon,  allows the creation of synthetics for different offsets and to perform different angle stacks. The source/receiver depths have to be provided. The offset range has also to be specified. The arrival times are calculated by ray-tracing through a horizontally ...",t:"5.5.1.2.3 Ray Tracing"},"261":{i:0.00205741238265642,u:"../content/analysis/layer_modeling/synthetic_and_property-log_generation/display.htm",a:"There are several display options within the Layer Modelling feature: By default the property logs are displayed regarding the block. When toggled on the   icon, the representation is one color per lithology. The property displayed is selected in the selection menu in the lower part of the window. ...",t:"5.5.1.2.4 Display"},"262":{i:0.00115179964307543,u:"../content/analysis/layer_modeling/synthetic_and_property-log_generation/layer_properties.htm",a:"Once the simulation has been run, the pseudo wells will have been generated and these well properties are then displayed in the lower section. The synthetics are also generated and display in the upper half of the window. When clicking on a given pseudo-well, a line appears to show the selected ...",t:"5.5.1.2.5 Layer Properties"},"263":{i:0.00115179964307543,u:"../content/analysis/layer_modeling/synthetic_and_property-log_generation/remove_layer.htm",a:"5.5.1.2.6 Remove Layer The existing layers of a model can be removed at any time, by right clicking on the left hand side pane, containing the simulation information. Thereafter, the regeneration of the pseudo-wells can be done by clicking on  .",t:"5.5.1.2.6 Remove Layer"},"264":{i:0.00115179964307543,u:"../content/analysis/layer_modeling/synthetic_and_property-log_generation/export_synthetic_datasets.htm",a:"The synthetic seismic data (both poststack and prestack), the layer property synthetics in Time (e.g. AI, Density etc.) and the stratigraphic levels/markers, from all modeling modules (i.e. Basic, Profile and Stochastic) can be exported along 2D lines. The stratigraphic levels/markers in the modeled ...",t:"5.5.1.2.7 Export Synthetic Datasets"},"265":{i:0.00115179964307543,u:"../content/analysis/layer_modeling/cross-plots.htm",a:"The cross-plot tool in the Basic layer modeling can be started from the icon   . It allows to analyze seismic and layer attributes from the modeled data. On the main window, the user select the attributes to be extracted and the extraction parameters.The extraction window is related to a reference ...",t:"5.5.1.3 Cross-Plots"},"266":{i:0.00115179964307543,u:"../content/analysis/layer_modeling/cross-plots/wavelet_scaling.htm",a:"If the wavelet has not been scaled to the real seismic, a pop up window will first appear prior to access to the attribute selection window. The user has 3 choices : [Start tool] to start the  Scale wavelet window [Mark scaled] if the wavelet is considered as scaled or does not need to be scaled ...",t:"5.5.1.3.1 Wavelet Scaling"},"267":{i:0.00115179964307543,u:"../content/analysis/layer_modeling/cross-plots/seismic_attributes.htm",a:"The seismic attribute selection/definition is comparable to the  main attribute window : the same attributes are available. An attribute can be selected in the list of Available types and add to the Defined attributes using the    button. The parameters of the attribute have to be specified. ...",t:"5.5.1.3.2 Seismic Attributes"},"268":{i:0.00115179964307543,u:"../content/analysis/layer_modeling/cross-plots/layer_attributes.htm",a:"For each modeled property, data can be extracted either along the log using a defined extraction window or by layers: Sliding: the property value is extracted within the extraction window where a calculation window slides along the well. The size of the calculation window is defined by the step ...",t:"5.5.1.3.3 Layer Attributes"},"269":{i:0.00115179964307543,u:"../content/processing.htm",a:"6 Processing",t:"6 Processing"},"270":{i:0.00138921643541111,u:"../content/processing/create_seismic_output.htm",a:"6.1 Create Seismic Output The Create Seismic Output option leads to a number of further choices, detailed in the following subsections:",t:"6.1 Create Seismic Output"},"271":{i:0.00272758662642617,u:"../content/processing/create_seismic_output/attributes.htm",a:"In this module any (attribute) volume can be calculated and saved to disk. In case of 2D attributes, the output is a new  data set. The volume output module can be run in batch mode, allowing to continue working in the main window while the processing is running. This module creates, for example, ...",t:"6.1.1 Attributes"},"272":{i:0.0012605815085403,u:"../content/processing/create_seismic_output/attributes/create_2d_output.htm",a:"A data set attribute can be created through the following procedure:  Processing \u003e Create Seismic output \u003e Attribute \u003e 2D First, Select the output quantity : it can be either a stored 2D volume or an attribute from the active 2D attribute set. Note that only attributes from the current attribute set ...",t:"6.1.1.1 2D - Create  Output"},"273":{i:0.0012605815085403,u:"../content/processing/create_seismic_output/attributes/create_3d_output.htm",a:"To create a 3D seismic output from an attribute, follow the path  Processing \u003e Create Seismic Output \u003e Attribute \u003e 3D. First, Select the output quantity. Optionally, a sub-volume can be specified. Note that only attributes from the current attribute set can be selected in the \u0027Select output ...",t:"6.1.1.2 3D - Create  Output"},"274":{i:0.00115179964307543,u:"../content/processing/create_seismic_output/attributes/multi_attribute.htm",a:"Multi-attribute output enables the user to create a volume with several attributes. \u0027Available Attributes\u0027 lists all possible components of the possible multi-attribute output and any combination can be selected. Once moved across into the \u0027Selected Attributes\u0027 list, these attributes are processed ...",t:"6.1.1.3 Multi Attribute"},"275":{i:0.00186816777469684,u:"../content/processing/create_seismic_output/attributes/multi-component_outputs.htm",a:"Multi-components cubes for some attributes (e.g Spectral Decomposition, Steering attributes) can be created. First, create your attribute in the attribute engine. Create the volume output: Go to  Processing \u003e Create Seismic Output \u003e Attribute \u003e Multi-cube data store and select a volume that contains ...",t:"6.1.1.4 Multi-Cube Data Store"},"276":{i:0.00115179964307543,u:"../content/processing/create_seismic_output/attributes/along_horizon.htm",a:"To create a seismic output in a time interval relative to a single horizon, the quantity to output has first to be selected from the list of stored data or attributes from the current attribute set. Specify the horizon and the Z interval relative to this horizon. A sub-area can be specified. The ...",t:"6.1.1.5 Along Horizon"},"277":{i:0.00115179964307543,u:"../content/processing/create_seismic_output/attributes/between_horizons.htm",a:"To create a seismic output between two horizons, first the quantity to output has to be selected from the list of stored data or attributes. Specify the horizons that form the upper and lower boundaries of the output volume. A Z shift to be specified can be applied to the upper boundary and/or to ...",t:"6.1.1.6 Between Horizons"},"278":{i:0.00115179964307543,u:"../content/processing/create_seismic_output/conversions_2d_to_3d.htm",a:"6.1.2 2D to 3D Conversions There are several ways to convert data between 2D and 3D:",t:"6.1.2 2D to 3D Conversions"},"279":{i:0.00224972053082758,u:"../content/processing/create_seismic_output/conversions_2d_to_3d/create_2d_grid.htm",a:"The Create 2D from 3D option is an interactive tool for creating 2D-lattices from a 3D volumes. This option can be used to create a 2D grid with a fixed grid spacing. When selected, the Create 2D Grid window is launched (see below). Here, specify the input 3D seismic volume and the output data set ...",t:"6.1.2.1 Create 2D Grid"},"280":{i:0.00115179964307543,u:"../content/processing/create_seismic_output/conversions_2d_to_3d/extract_2d_from_3d.htm",a:"Extract 3D data onto selected 2D lines. Input data is required in the form of a stored 3D volume. One or more 2D lines can be selected for the 3D data to be extracted onto. The output data set requires naming: If just one line is selected, you may also sub-select a trace range.",t:"6.1.2.2 Extract 2D from 3D"},"281":{i:0.00164131803766736,u:"../content/processing/create_seismic_output/angle_mute_function.htm",a:"This module creates angle-based Z-Offset functions from velocity volumes. The primary input is a velocity model that provides the time-depth relation. If you have no velocity volume available, click \u0027Create\u0027. This will bring up a window in which you can select a volume and tag it with a velocity ...",t:"6.1.3 Angle Mute Function"},"282":{i:0.00115179964307543,u:"../content/processing/create_seismic_output/angle_mute_function/advanced_angle_mute_parameters.htm",a:"The ray-tracing can be performed in two ways: Simple: The ray is going directly from the source to the depth of the target layer, and up to the receiver in the same way. This does not account for ray bending, or velocity inversions. Advanced: Will honour the ray bending according to Snell\u0027s law and ...",t:"6.1.3.1 Advanced Angle Mute Paramaters"},});