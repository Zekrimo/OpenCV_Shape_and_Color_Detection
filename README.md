voor het compileren van de code volg de onderstaande instructies.
Om te beginnen installeren we de benodigde afhankelijkheden om de code correct te laten werken.

afhankelijkheden:

	Hardware:
		Webcam (intergrated of extern)

	Operating system:
		Ubuntu Linux - Jammy Jellyfish (22.04)

	Software:
		G++
		ros-humble-ros-base: https://docs.ros.org/en/humble/Installation/Ubuntu-Install-Debians.html

		eclipse ide for embedded c/c++ developers: https://www.eclipse.org/downloads/

Instructies:

	Stap 1:
		Download de code uit de Main als .zip op : https://bitbucket.aimsites.nl/projects/WORR_CV_SHG/repos/opencv-bp/browse
		
	Stap 2:
		Maak een nieuwe map aan in je eclipse workspace en plaats de inhoud van de .zip in de map.

	Stap 3:
		Start eclipse ide for embedded c/c++

	Stap 4: 
		Ga naar 'File' -> 'New' -> 'CMake Project with Existing code' . Er zal nu een nieuw venster openen.

	Stap 5:
		Zet 'existing code location' naar de locatie van de uitgepakte .zip en klik op 'Finish'.

	Stap 6:
		Open een terminal in de Root van het project
	
	Stap: 7:
		voer het commando "cmake ./" uit.
	
	Stap 8:
		voer het command "make" uit.





