appgraph
========

Applied Graphics / LSU / CSC 4357

Overview
--------

Semester long project that consisted of creating a particle engine
using a geometry shader to convert points to triangles and texturing
the triangles.

TODO
----

Fix the cross-platform issues.

* Original intent was to have code running on Windows, Linux, and Mac OSX
	* Windows fails to build in VS 2013
		* SDL error on my part
	* Linux builds and renders
	* Mac OSX 10.9 Mavericks builds and renders
* Possible video driver
	* ATI Catalyst on Linux appears to not render correctly
* Learn the mathematics behind fluid dynamics and pseudorandom geometry
* Figure out how to get lifetime of particles to work in this system
* Convert to using compute shader/SSBOs at a later date and possibly CUDA
* Add an FPS counter

Details
-------

* 450000 particles