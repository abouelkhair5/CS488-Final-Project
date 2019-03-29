-- Scene file to demonstrate the features implemented for ray tracer for CS488

mat_white = gr.material({1.0, 1.0, 1.0}, {0, 0, 0}, 0)
mat_green = gr.material({0.0, 1.0, 0.0}, {0, 0, 0}, 0)
mat_blue  = gr.material({0.0, 0.0, 1.0}, {0, 0, 0}, 0)
mat_red   = gr.material({1.0, 0.0, 0.0}, {0, 0, 0}, 0)

white_light = gr.light({0.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})

scene = gr.node('scene')

-- the floor
floor = gr.mesh( 'floor', 'plane.obj' )
scene:add_child(floor)
floor:set_material(mat_white)
floor:scale(30, 30, 30)

-- the right wall
right_wall = gr.mesh( 'right_wall', 'plane.obj' )
scene:add_child(right_wall)
right_wall:set_material(mat_red)
right_wall:scale(30, 30, 30)
right_wall:rotate('Z', -90)
right_wall:translate({100, 0, 0})

-- the left wall
left_wall = gr.mesh( 'left_wall', 'plane.obj' )
scene:add_child(left_wall)
left_wall:set_material(mat_green)
left_wall:scale(30, 30, 30)
left_wall:rotate('Z', 90)
left_wall:translate({-100, 0, 0})

-- the back wall
back_wall = gr.mesh( 'back_wall', 'plane.obj' )
scene:add_child(back_wall)
back_wall:set_material(mat_white)
back_wall:scale(30, 30, 30)
back_wall:rotate('X', -90)
back_wall:translate({0, 0, -100})

gr.render(scene, 'demo.png', 512, 512,
        {0, 0, 100}, {0, 0, -100}, {0, 1, 0}, 50,
        {0.4, 0.4, 0.4}, {white_light})
