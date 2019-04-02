-- Scene file to demonstrate the features implemented for ray tracer for CS488

mat_white = gr.material({1.0, 1.0, 1.0}, {0, 0, 0}, 0, false)
mat_green = gr.material({0.0, 1.0, 0.0}, {0, 0, 0}, 0, false)
mat_blue  = gr.material({0.0, 0.0, 1.0}, {0, 0, 0}, 0, false)
mat_red   = gr.material({1.0, 0.0, 0.0}, {0, 0, 0}, 0, false)
mirror    = gr.material({0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, 100, false)
glass     = gr.trans_material({0.0, 0.0, 0.0}, {0.5, 0.5, 0.5}, {1.0, 1.0, 1.0}, 25, 1.52, false)
water     = gr.trans_material({0.0, 0.0, 0.05}, {0.7, 0.7, 0.7}, {1.0, 1.0, 1.0}, 25, 1.33, false)
air       = gr.trans_material({0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, 25, 1.02, false)
wallpaper = gr.texture("world.png")
bumps     = gr.bump_map("bump_map.png", {1.0, 1.0, 1.0}, {0.5, 0.5, 0.5}, 25)
perlin    = gr.random_texture({1.0, 1.0, 1.0}, 20)
wood      = gr.wood({0.65, 0.4, 0.01}, 50)
marble    = gr.marble({1.0, 1.0, 1.0}, 1, 2, 0.6)
sand      = gr.texture("textures/sand_01_diff_2k.png")
sandstone = gr.texture("textures/sandstone_blocks_04_diff_2k.png")

white_light = gr.area_light({0.0, 80.0, 20.0}, {0.9, 0.9, 0.9}, {1, 0, 0}, 4, 1)
point_light = gr.light({-10.0, 80.0, 20.0}, {0.9, 0.9, 0.9}, {1, 0, 0})

scene = gr.node('scene')

-- the floor
floor = gr.mesh( 'floor', 'plane.obj' )
scene:add_child(floor)
floor:set_material(sand)
floor:scale(300, 300, 300)
floor:translate(0, -30, 0)

box = gr.mesh('khofo', 'pyramid.obj')
scene:add_child(box)
box:scale(30, 30, 15)
box:set_material(sandstone)
box:rotate('X', -90)
box:translate(-30, -30, -30)

khafra = gr.mesh('khafra', 'pyramid.obj')
scene:add_child(khafra)
khafra:scale(20, 20, 10)
khafra:set_material(sandstone)
khafra:rotate('X', -90)
khafra:translate(0, -30, 0)


mankawra = gr.mesh('mankawra', 'pyramid.obj')
scene:add_child(mankawra)
mankawra:scale(12, 12, 6)
mankawra:set_material(sandstone)
mankawra:rotate('X', -90)
mankawra:translate(20, -30, 15)
for i = 0, 21, 1 do
    scene:rotate('Y', 15)
end

for i = 22, 24, 1 do
    scene:rotate('Y', 15)
    gr.render(scene, 'example/final_scene/'..i..'.png', 1024, 1024,
            {0, 10, 100}, {0, -30, -100}, {0, 1, 0}, 50,
            {0.4, 0.4, 0.4}, {white_light})
end
