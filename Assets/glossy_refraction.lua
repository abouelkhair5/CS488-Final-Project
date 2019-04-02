-- Scene file to demonstrate the features implemented for ray tracer for CS488

mat_white = gr.material({1.0, 1.0, 1.0}, {0, 0, 0}, 0, false)
mat_green = gr.material({0.0, 1.0, 0.0}, {0, 0, 0}, 0, false)
mat_blue  = gr.material({0.0, 0.0, 1.0}, {0, 0, 0}, 0, false)
mat_red   = gr.material({1.0, 0.0, 0.0}, {0, 0, 0}, 0, false)
mirror    = gr.material({0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, 100, false)
glass     = gr.trans_material({0.0, 0.0, 0.0}, {0.5, 0.5, 0.5}, {1.0, 1.0, 1.0}, 25, 1.52, true)
water     = gr.trans_material({0.0, 0.0, 0.05}, {0.7, 0.7, 0.7}, {1.0, 1.0, 1.0}, 25, 1.33, true)
air       = gr.trans_material({0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, 25, 1.02, false)
wallpaper = gr.texture("world.png")
bumps     = gr.bump_map("bump_map.png", {1.0, 1.0, 1.0}, {0.5, 0.5, 0.5}, 25)
perlin    = gr.random_texture({1.0, 1.0, 1.0}, 20)
wood      = gr.wood({0.65, 0.4, 0.01}, 50)
marble    = gr.marble({1.0, 1.0, 1.0}, 1, 2, 0.6)

white_light = gr.area_light({0.0, 80.0, 20.0}, {0.9, 0.9, 0.9}, {1, 0, 0}, 4, 1)
point_light = gr.light({-10.0, 80.0, 20.0}, {0.9, 0.9, 0.9}, {1, 0, 0})

scene = gr.node('scene')

-- the floor
floor = gr.mesh( 'floor', 'plane.obj' )
scene:add_child(floor)
floor:set_material(mat_white)
floor:scale(30, 30, 30)
floor:translate(0, -30, 0)

-- the right wall
right_wall = gr.mesh( 'right_wall', 'plane.obj' )
scene:add_child(right_wall)
right_wall:set_material(mat_red)
right_wall:scale(30, 30, 30)
right_wall:rotate('Z', -90)
right_wall:translate(20, 0, 0)

-- the left wall
left_wall = gr.mesh( 'left_wall', 'plane.obj' )
scene:add_child(left_wall)
left_wall:set_material(mat_green)
left_wall:scale(30, 30, 30)
left_wall:rotate('Z', 90)
left_wall:translate(-20, 0, 0)

-- the back wall
back_wall = gr.mesh( 'back_wall', 'plane.obj' )
scene:add_child(back_wall)
back_wall:set_material(mat_white)
back_wall:scale(30, 30, 30)
back_wall:rotate('X', -90)
back_wall:translate(0, 0, -30)

-- the object (sphere)
ball = gr.sphere('b')
--scene:add_child(ball)
ball:set_material(water)
ball:scale(7, 7, 7)
ball:translate(13, 5, -10);

ball2 = gr.sphere('b')
scene:add_child(ball2)
ball2:set_material(water)
ball2:scale(7, 7, 7)
ball2:translate(-13, -22.9, -10);

box = gr.cube('x')
--scene:add_child(box)
box:scale(10, 10, 10)
box:set_material(glass)
box:translate(8, -15, 0)

cylinder = gr.cylinder('x')
--scene:add_child(cylinder)
cylinder:scale(5, 15, 5)
cylinder:set_material(water)
cylinder:translate(-12, 0, 0)

gr.render(scene, 'example/glossy_refraction_water4.png', 1024, 1024,
        {0, 10, 65}, {0, -15, -60}, {0, 1, 0}, 50,
        {0.4, 0.4, 0.4}, {white_light})
