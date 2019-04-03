-- Scene file to demonstrate the features implemented for ray tracer for CS488

mat_white = gr.material({1.0, 1.0, 1.0}, {0, 0, 0}, 0, false)
mat_green = gr.material({0.0, 1.0, 0.0}, {0, 0, 0}, 0, false)
mat_blue  = gr.material({0.0, 0.0, 1.0}, {0, 0, 0}, 0, false)
mat_red   = gr.material({1.0, 0.0, 0.0}, {0, 0, 0}, 0, false)
mirror    = gr.material({0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, 100, false)
wallpaper = gr.texture("world.png")
bumps     = gr.bump_map("bump_map.png", {1.0, 1.0, 1.0}, {0.0, 0.0, 0.0}, 25)

white_light = gr.area_light({0.0, 80.0, 20.0}, {0.9, 0.9, 0.9}, {1, 0, 0}, 4, 1)

scene = gr.node('scene')

-- the object (sphere)
ball = gr.sphere('b')
scene:add_child(ball)
ball:set_material(bumps)
ball:scale(10, 10, 10)

for x=-10,10,1 do
    white_light = gr.area_light({x, 0.0, 30.0}, {0.9, 0.9, 0.9}, {1, 0, 0}, 1, 0)
    gr.render(scene, 'example/moving_bump/bump_demo'..(10 + x)..'.png', 1024, 1024,
            {0, 10, 65}, {0, -15, -60}, {0, 1, 0}, 50,
            {0.4, 0.4, 0.4}, {white_light})
end
