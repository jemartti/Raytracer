-- A simple custom scene demonstrating all my code

mat1  = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 100)
mat2  = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
stone = gr.material({0.8, 0.7, 0.7}, {0.0, 0.0, 0.0}, 25)

scene_root = gr.node('root')

s1 = gr.nh_sphere('s1', {0, -1200, -500}, 1000)
scene_root:add_child(s1)
s1:set_material(mat1)

s2 = gr.nh_box('s2', {0, 100, 0}, 100)
scene_root:add_child(s2)
s2:set_material(mat2)

require('dodeca')
scene_root:add_child(dodeca)
dodeca:set_material(stone)
dodeca:translate(-2.4, 0, -0.4)
dodeca:scale(50, 50, 50)

white_light = gr.light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
orange_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'sample.png', 500, 500,
	  {0, 0, 800}, {0, 0, -800}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light, orange_light})
