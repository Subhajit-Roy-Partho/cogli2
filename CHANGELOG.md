# Changelog

## 2.3.0 [r134]

* Update glfw to make the code work also on Wayland
* Fix a bug whereby the code would segfault on exit
* Fix a bug whereby the particle indexes were not correctly set
* Update the povray export for patchy particles

## 2.2.0 [r121]

* Add two ways of identifying the 3' and 5' ends of DNA and RNA strands. This can be done with the new `--end-handling, -e` option or by pressing the `e` key during the rendering
* Add the possibility of specifying group of shapes in an mgl file. Update the README accordingly
* Add the possibility of drawing Ellipsoidal Particles from an MGL file

## 2.1.0

* Add a new parser to handle "patchy-swap" configurations
* Add a new action that can be used to hide/show particles
* Improve the performance by sorting only when transparent shapes are present
* Make it possible to set custom colors for the different species with the PatchySwap parser
* Add the --drums-full switch to apply the DRUMS scheme to the whole nucleotide (as opposed to --drums, which colors only the bases)
* Add the --no-box option to hide the box from the command line
* Add spherical caps to the patches
* Bugfixing:
	* Fix a bug whereby exporting scenes containing patch-less PatchyParticles to povray would segfault
	* Fix a bug whereby the DNA parser would fail if the topology file didn't end with an empty line
	* The colours of the axes' keys (whose appearance can be toggled with 'k') do not change with the world's lights
	* Fix a bug that would prevent compilation with some compilers

## 2.0.1 

* Improved the opengl -> povray mapping by setting povray's assumed_gamma to 2.2
* Fixed a bug that would prevent the fps counter from sometimes showing up on the screen
* Fixed a bug that would make the code segfault whenever a DNA topology file would contain fewer lines than what stated in the header line

## 2.0.0

* Ported the OpenGL engine to *modern* OpenGL. The performance have been greatly enhanced.
* Added an option to display/hide the axes keys on the bottom left of the viewport (k key)
* Added an option to display particle ids on screen (l key, works only in perspective mode)
* The fps counter is now rendered in the viewport
* Improved the OpenGL -> povray mapping (especially the lighting)
