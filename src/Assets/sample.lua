-- test for hierarchical ray-tracers.
-- Thomas Pflaum 1996

gold = gr.material({0.9, 0.8, 0.4}, {0.8, 0.8, 0.4}, 25)
grass = gr.material({0.1, 0.7, 0.1}, {0.0, 0.0, 0.0}, 0)
blue = gr.material({0.7, 0.6, 1}, {0.5, 0.4, 0.8}, 25)
hide = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, 20)
mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 45)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25)

scene = gr.node('scene')
scene:rotate('X', 23)
scene:translate(0, 0, -15)

s1 = gr.nh_sphere('s4', {4, 1, 6}, 3)
scene:add_child(s1)
s1:set_material(mat1)

-- the arc
arc = gr.node('arc')
scene:add_child(arc)
arc:translate(0,0,-8)
arc:rotate('Y', 50)
arc:translate(4,0,8)
p1 = gr.cube('p1')
arc:add_child(p1)
p1:set_material(gold)
p1:scale(0.8, 4, 0.8)
p1:translate(-2.4, 0, -0.4)

p2 = gr.cube('p2')
arc:add_child(p2)
p2:set_material(gold)
p2:scale(0.8, 4, 0.8)
p2:translate(1.6, 0, -0.4)

s = gr.sphere('s')
arc:add_child(s)
s:set_material(gold)
s:scale(4, 0.6, 0.6)
s:translate(0, 4, 0)

cow_poly = gr.mesh('cow', 'cow.obj')
factor = 2.0/(2.76+3.637)

cow_poly:set_material(hide)

cow_poly:translate(0.0, 2, 0.0)
cow_poly:scale(factor, factor, factor)
cow_poly:translate(0.0, 0.5, 0.0)
cow_poly:rotate('Y', -90)

arc:add_child(cow_poly)

-- the floor
plane = gr.mesh( 'plane', 'plane.obj' )
scene:add_child(plane)
plane:set_material(grass)
plane:scale(30, 30, 30)

steldodec = gr.mesh( 'dodec', 'dodeca.obj' )
steldodec:set_material(mat3)
-- steldodec:translate(-10, -200, 0);
steldodec:translate(6, 5, -10)
scene:add_child(steldodec)

-- The lights
l1 = gr.light({200,200,400}, {0.8, 0.8, 0.8}, {1, 0, 0})
l2 = gr.light({0, 5, -20}, {0.4, 0.4, 0.8}, {1, 0, 0})

gr.render(scene, 'sample.png', 512, 512, 
	  {0, 0, 0,}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4}, {l1, l2})
