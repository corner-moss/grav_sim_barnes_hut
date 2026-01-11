# TODO
## fixes
-[] Correct orbit init function
-[x] potential normalization
-[] compile time shader paths?
-[] the tick / frame timing system
-[] after barnes hut make direct sum compatible
-[] fix graphics system

## future
-[x] barnes-hut method
-[] 3D camera
-[] more sensible rendering pipeline -> some way to store shader references and vaos in e.g. renderer struct and some way to then enable disable rendering of certain elements
-[] fast circle rendering -> minimally for central mass or similar ideally for every point 
-[] fast multipole method
-[] better initializations
-[] dedicated files for all methods and single binary with command line option or similar -> would enable tests and demos

## optionals
-[] tests and demos
-[] interaction through cursor etc.
-[] collision detection
-[] more complex rendering e.g. planets, background, mass density colouring etc.
-[] graphics math mat4 stuff / quarternions

## optimizations
-[] the tree renderer
-[] parallel force calculation
-[] maybe parallel tree construction possible with prior sorting
-[] parallel propagation / position updates ? maybe for large number of masses, depends on overhead
-[] implicit depths for chained single children nodes through counter / node size. Would give theoretical guarantee for a maximum 2*n+1 nodes as each inserted mass can create at most one additional internal node. Would complicate insertions as it might be necessary to insert nodes in between existing ones. To keep the current method of propagation that relies on the fact that all nodes lie at higher indices than their parents large amounts of copying would be necessary in the worst case. Probably better to traverse tree then with parent references in nodes.
