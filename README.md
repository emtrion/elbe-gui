**Documentation elbeFrontend**
	
elbeFrontend is a desktop application to manage and use elbe-rfs. The idea is to make things easier when using elbe, especially for people who are not comfortable using a terminal.

Features:
	•create projects (in initVM and as elbefrontend Project) 
	•create, import and edit elbe XML files
	•validate XML files against the dbsfed.xsd from elbe
	•build projects (using elbe as outside process)
	•download files from initVM
	•delete projects (from initVM and filesystem)

First Steps:
* At first you are asked to specify an elbe and an initVM 
* to create a new project go to File->New->New Project, fill the wizard and confirm
* to add a xml file to your project you can either import one by going to File->Import or create a new one by going to File->New->New XML
* you can now edit the xml in the editor
* the file will be validated everytime you save the file or you can do that manually by going to Elbe->Validate
* when you are ready to build your project you go to Elbe->Build project
* you can select outputfiles which will be downloaded directly after the build
* to delete a project you go to File->Delete, select the project you want to delete and confirm

	
Currently, only the versions 2.0 - 2.4 are fully supported. It might work with other versions but functionality is not guaranteed.  
	
Todo:
* create an initVM  from frontend
* right-click context menu
* multi-selection in delete dialog



