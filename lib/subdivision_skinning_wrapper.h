#ifndef __subdivision_skinning_wrapper_h__
#define __subdivision_skinning_wrapper_h__

typedef float subdivision_evaluator_real_t;

/*
Given a control mesh with 'num_vertices' 3D vertices,
3*num_vertices values 'vertices' arranged:
    x0 y0 z0 x1 y1 z1 ...
'num_faces' triangle or quad faces,
4*num_faces indices into the num_vertices arranged:
    face0_vertex0 face0_vertex1 face0_vertex2 face0_vertex3 face1_vertex0 face1_vertex1 face1_vertex2 face1_vertex3 ...
and a positive integer level representing the number of subdivision steps to perform (e.g. 4),
creates and returns an opaque subdivision_evaluator*.
When finished, the memory used by the subdivision_evaluator* must be freed using delete_subdivision_evaluator().
NOTE: For a triangle face, set the face's last vertex to -1.
*/
void* new_subdivision_evaluator( int num_vertices, const subdivision_evaluator_real_t* vertices, int num_faces, const int* faces, int level );
// Frees the memory associated with the subdivision_evaluator created by new_subdivision_evaluator().
void delete_subdivision_evaluator( void* subdivision_evaluator );

// Returns the number of quad faces generated by the subdivision_evaluator.
int num_refined_quad_faces_of_subdivision_evaluator( const void* subdivision_evaluator );
/*
Given a 'subdivision_evaluator',
upon return, places 'num_refined_quad_faces_of_subdivision_evaluator()' quad faces in 'refined_quad_faces_out'.
The quads are arranged:
    face0_vertex0 face0_vertex1 face0_vertex2 face0_vertex3 face1_vertex0 face1_vertex1 face1_vertex2 face1_vertex3 ...
NOTE: The 'refined_quad_faces_out' parameter must point to (at least) 4*num_refined_quad_faces_of_subdivision_evaluator() integers of memory.
NOTE: All faces are quads, so there will be no -1 indices.
      This is in contrast to new_subdivision_evaluator(), which can take triangles or quads.
*/
void get_refined_quad_faces_of_subdivision_evaluator( const void* subdivision_evaluator, int* refined_quad_faces_out );

// Returns the number of refined vertices generated by the subdivision_evaluator.
int num_refined_vertices_of_subdivision_evaluator( const void* subdivision_evaluator );
/*
Given a 'subdivision_evaluator',
upon return, places 'num_refined_vertices_of_subdivision_evaluator()' 3D vertex values in 'refined_vertices_out'.
The vertices are arranged:
    x0 y0 z0 x1 y1 z1 ...
NOTE: The 'refined_vertices_out' parameter must point to (at least) 3*num_refined_vertices_of_subdivision_evaluator() subdivision_evaluator_real_t of memory.
*/
void get_refined_vertices_of_subdivision_evaluator( const void* subdivision_evaluator, subdivision_evaluator_real_t* refined_vertices_out );
/*
Given a 'subdivision_evaluator' and
a 'vertex_dimension'-dimensional control mesh 'control_vertices' of the same quantity
and arrangement as was passed to new_subdivision_evaluator():
    (in 3D) x0 y0 z0 x1 y1 z1 ...
    (in general) vertex0_coordinate0 vertex0_coordinate1 ... vertex0_coordinate_vertex_dimension-1 vertex1_coordinate0 vertex1_coordinate1 ...
Upon return, places 'num_refined_vertices_of_subdivision_evaluator()' 'vertex_dimension'-dimensional
vertex values in 'refined_vertices_out'. The vertices are arranged:
    (in 3D) x0 y0 z0 x1 y1 z1 ...
    (in general) vertex0_coordinate0 vertex0_coordinate1 ... vertex0_coordinate_vertex_dimension-1 vertex1_coordinate0 vertex1_coordinate1 ...
NOTE: The 'refined_vertices_out' parameter must point to (at least) 'vertex_dimension*num_refined_vertices_of_subdivision_evaluator()' subdivision_evaluator_real_t of memory.
NOTE: 'vertex_dimension' must be positive (greater than zero).
*/
void get_refined_vertices_of_subdivision_evaluator_with_control_vertices( const void* subdivision_evaluator, int vertex_dimension, const subdivision_evaluator_real_t* control_vertices, subdivision_evaluator_real_t* refined_vertices_out );

/*
Given a subdivision_evaluator* and num_refined_vertices_of_subdivision_evaluator()-by-'num_transforms'
'weights', which are values arranged:
    vertex0_transform0 vertex0_transform1 vertex0_transform2 ... vertex1_transform0 vertex1_transform1 vertex1_transform2 ...
creates and returns an opaque subdivision_skinning_engine*.
When finished, the memory must be freed by delete_subdivision_skinning_engine().
NOTE: The subdivision_skinning_engine does *not* depend on the 'subdivision_evaluator'.
      You may call delete_subdivision_evaluator() to free the subdivision_evaluator
      once you have created the subdivision_skinning_engine().
*/
void* new_subdivision_skinning_engine( const void* subdivision_evaluator, int num_transforms, const subdivision_evaluator_real_t* weights );
// Frees the memory associated with the subdivision_skinning_engine created by new_subdivision_skinning_engine().
void delete_subdivision_skinning_engine( void* subdivision_skinning_engine );

/*
Given a subdivision_skinning_engine* and
a set of 'transforms',
returns in 'control_mesh_vertices_out' 3D control mesh vertices
that provide the best fitting refined subdivision surface to the linear blend
skin deformation specified by the weights and 'transforms'.
The parameter 'transforms' contains 4x4 matrices stacked vertically into a very tall matrix.
The matrix is provided in row-major order, and the number of transforms
is the parameter 'num_transforms' passed to new_subdivision_skinning_engine().
*/
void compute_control_mesh_vertices_given_transforms_for_subdivision_skinning_engine( const void* subdivision_skinning_engine, const subdivision_evaluator_real_t* transforms, subdivision_evaluator_real_t* control_mesh_vertices_out );

// Given a 'subdivision_skinning_engine',
// returns the number of refined vertices generated by the subdivision_skinning_engine.
// NOTE: The returned value will be the same as would be returned by
//       'num_refined_vertices_of_subdivision_evaluator()' with the 'subdivision_evaluator'
//       used to create the 'subdivision_skinning_engine'.
int num_refined_vertices_of_subdivision_skinning_engine( const void* subdivision_skinning_engine );
/*
Given a 'subdivision_skinning_engine' and
a 'vertex_dimension'-dimensional control mesh 'control_vertices' of the same quantity
and arrangement as was passed to new_subdivision_evaluator():
    (in 3D) x0 y0 z0 x1 y1 z1 ...
    (in general) vertex0_coordinate0 vertex0_coordinate1 ... vertex0_coordinate_vertex_dimension-1 vertex1_coordinate0 vertex1_coordinate1 ...
Upon return, places 'num_refined_vertices_of_subdivision_skinning_engine()' 'vertex_dimension'-dimensional
vertex values in 'refined_vertices_out'. The vertices are arranged:
    (in 3D) x0 y0 z0 x1 y1 z1 ...
    (in general) vertex0_coordinate0 vertex0_coordinate1 ... vertex0_coordinate_vertex_dimension-1 vertex1_coordinate0 vertex1_coordinate1 ...
NOTE: The 'refined_vertices_out' parameter must point to (at least) 'vertex_dimension*num_refined_vertices_of_subdivision_skinning_engine()' subdivision_evaluator_real_t of memory.
NOTE: 'vertex_dimension' must be positive (greater than zero).
*/
void get_refined_vertices_of_subdivision_skinning_engine_with_control_vertices( const void* subdivision_skinning_engine, int vertex_dimension, const subdivision_evaluator_real_t* control_vertices, subdivision_evaluator_real_t* refined_vertices_out );

#endif /* __subdivision_skinning_wrapper_h__ */
