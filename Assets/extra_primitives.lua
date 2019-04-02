-- Scene file to demonstrate the features implemented for ray tracer for CS488

mat_white = gr.material({1.0, 1.0, 1.0}, {0, 0, 0}, 0, false)
mat_green = gr.material({0.0, 1.0, 0.0}, {0, 0, 0}, 0, false)
mat_blue  = gr.material({0.0, 0.0, 1.0}, {0, 0, 0}, 0, false)
mat_red   = gr.material({1.0, 0.0, 0.0}, {0, 0, 0}, 0, false)
mirror    = gr.material({0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, 100, false)
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
floor:set_material(perlin)
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

-- the object (cone)
cone = gr.cone("c")
scene:add_child(cone)
cone:set_material(mat_blue)
cone:rotate('z', -45)
cone:scale(7, 7, 7)
cone:translate(-15, -30, -10)
cone2 = gr.cone("c")
scene:add_child(cone2)
cone2:set_material(mat_green)
cone2:rotate('z', 180)
cone2:scale(7, 7, 7)
cone2:translate(6, -23, -7)

-- object (cylinder)
cylinder = gr.cylinder("cy")
scene:add_child(cylinder)
cylinder:set_material(wood)
cylinder:scale(5, 5, 5)
cylinder:translate(12, -30, 8)

gr.render(scene, 'example/extra_primitives.png', 1024, 1024,
        {0, 10, 65}, {0, -15, -60}, {0, 1, 0}, 50,
        {0.4, 0.4, 0.4}, {white_light})
