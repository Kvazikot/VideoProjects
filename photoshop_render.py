import win32com.client
import os
import cv2
import photoshop.api as ps
#pip install photoshop_python_api

example_text = "В известном мысленном эксперименте Джона Уиллера \r \
с двумя щелями и отложенном выборе был предложена[1] проверка гипотезы \r о том что прошлое можно изменить стирая информацию о наблюдении в будущем."



psApp = win32com.client.Dispatch("Photoshop.Application")
psApp.Open(r"D:\\projects\\VideoProjects\\Prob_Terminator_scenario\\terminator_text.psd")
doc = psApp.Application.ActiveDocument
layer_facts = doc.ArtLayers["Facts"]
layer_rect = doc.ArtLayers["rect1"]
text_of_layer = layer_facts.TextItem
print(f"current text position is {text_of_layer.position}")
print(f"rect bounds is {layer_rect.bounds}")
print(f"text bounds is {layer_facts.bounds}")

bounds = layer_rect.bounds
text_of_layer.size = 6
text_of_layer.contents = example_text
text_width = bounds[2] - bounds[0]
text_height = bounds[3] - bounds[1]
text_of_layer.position = (bounds[0]+text_width/2,bounds[3]-text_height/2)
#RasterizeType = win32com.client.Dispatch('Photoshop.RasterizeType')
print(ps.LayerKind.TextLayer)
new_text_layer = new_doc
new_text_layer.kind = ps.LayerKind.TextLayer

text_of_layer.rasterize(ps.RasterizeType.TEXTCONTENTS)
options = win32com.client.Dispatch('Photoshop.ExportOptionsSaveForWeb')
options.Format = 13   # PNG Format
options.PNG8 = False  # Sets it to PNG-24 bit

fourcc = cv2.VideoWriter_fourcc(*'mp4v')
out = cv2.VideoWriter('render_opacity&blur.avi',fourcc, 25.0, (1280,720))

#psApp.Application.ActiveDocument.rasterizeAllLayers()

n_total_frames = 10
for frame in range(0,n_total_frames,1):
	layer_facts.opacity = (100 * (frame+1) / (n_total_frames));
	#layer_facts.applyMotionBlur(120,10)
	#layer_facts.applyClouds()
	print(f"frame={frame} layer_facts.opacity={layer_facts.opacity}")
	pngfile = f"D:\\projects\\VideoProjects\\Prob_Terminator_scenario\\render_output\\test.png"
	doc.Export(ExportIn=pngfile, ExportAs=2, Options=options)
	final = cv2.imread(pngfile)
	out.write(final)

out.release()

#text_of_layer.contents = "Costum text from Python"
print(text_of_layer.contents)

#doc.ResizeImage(Width=2048,
#                Height=2048,
#                Resolution=300,
#                ResampleMethod=8)
#layerPs = doc.ArtLayers["Name_of_Layer_Here"]
#layerPs.visible = False # Or True if you want to unhide it.
