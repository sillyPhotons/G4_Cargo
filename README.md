<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/sillyPhotons/G4_Cargo">
    <img src="images/logo.png" alt="Logo" width = 500>
  </a>

  <h3 align="center">Geant4 Cargo Scanning Monte-Carlo Simulation</h3>

An aluminum cargo container containing objects of varying density and atomic number is exposed to mega-voltage gammas, neutrons, and muons, and the interaction of these particles with matter is analyzed qualitatively. 
    <br />
</p>

<!-- TABLE OF CONTENTS -->
## Table of Contents
* [Implementations](#implementation)
* [Project Report](#about-the-project)
* [Contact](#contact)

## Implementations

In addition to the required concrete classes, I implement
  * Command based scoring meshes to score energy deposition
  * Sensitive detector class to score particle interaction position
  * Hits class to store relevant hit information
  * RunAction class for booking histograms 
  * EventAction class for filling histograms
  
Simulation results are visualized using `ROOT @ cern` and the `Python` language.

## Project Report
A brief project report that provide a brief description of the parts of this simulation, the primary references used, and also some acknowledgements can be found, along with its latex source code used for compilation in OverLeaf, in the `Report` folder in the repository.


## Contact

Ruiheng Su - ruihengsu@alumni.ubc.ca

Project Link: [https://github.com/sillyPhotons/G4_Cargo](https://github.com/sillyPhotons/G4_Cargo)
