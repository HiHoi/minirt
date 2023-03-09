#ifndef  STRUCTURES_H
# define STRUCTURES_H

typedef struct s_ray t_ray;

typedef struct s_vec3 t_vec3;
typedef struct s_vec3 t_point3;
typedef struct s_vec3 t_color3;

typedef struct s_camera t_camera;
typedef struct s_canvas t_canvas;

typedef struct s_hit_record	t_hit_record;

typedef struct s_sphere	t_sphere;
typedef	struct s_cylinder t_cylinder;
typedef struct s_plane t_plane;
typedef	struct s_object	t_object;

typedef	struct s_light t_light;

typedef	struct s_scene t_scene;
typedef struct s_material	t_material;

# define FALSE 0
# define TRUE 1

typedef int	t_object_type;
# define SP 0
# define LIGHT_POINT 1
# define CYLINDER 2
# define PLANE 3

# define EPSILON 1e-6
# define LUMEN 3

typedef int	t_bool;

typedef struct s_game
{
	void	*mlx;
	void	*win;
}	t_game;

struct s_vec3
{
	double  x;
	double  y;
	double  z;
};

struct s_ray
{
	t_point3    orig;
	t_vec3      dir;
};

struct s_camera
{
	t_point3    orig;
	double      viewport_w;
	double      viewport_h;
	t_vec3      horizontal;
	t_vec3      vertical;
	double      focal_len;
	t_point3    left_bottom;
};

struct s_canvas
{
	int     width;
	int     height;
	double  aspect_ratio;
};

struct s_object
{
	t_object_type	type;
	t_color3		albedo;
	void			*element;
	void			*next;
};

struct s_hit_record
{
	t_point3	p;
	t_vec3		normal;
	double		tmin;
	double		tmax;
	double		t;
	t_bool		front_face;
	t_color3	albedo;
};

struct s_scene
{
	t_canvas		canvas;
	t_camera		camera;
	t_object		*world;
	t_object		*light;
	t_color3		ambient;
	double			ambient_ratio;
	t_ray			ray;
	t_hit_record	rec;
};

struct s_sphere
{
	t_point3	center;
	double		radius;
	double		radius2;
};

struct s_cylinder
{
	t_point3	center;
	t_vec3		dir;
	double		radius;
	double		height;
};

struct s_plane
{
	t_point3	center;
	t_vec3		dir;
};

struct s_light
{
	t_point3	origin;
	t_color3	light_color;
	double		bright_ratio;
};

#endif