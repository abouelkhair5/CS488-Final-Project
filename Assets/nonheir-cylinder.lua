-- A simple scene with some miscellaneous geometry.

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)

scene_root = gr.node('root')

c1 = gr.nh_cylinder('c1', { 0, 0, -400}, 100, 100, 200)
scene_root:add_child(c1)
c1:set_material(mat1)

-- A small stellated dodecahedron.

white_light = gr.light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
magenta_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'nonhier-cylinder.png', 512, 512,
        {0, 0, 800}, {0, 0, -1}, {0, 1, 0}, 50,
        {0.3, 0.3, 0.3}, {white_light, magenta_light})