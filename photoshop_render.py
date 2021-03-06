import win32com.client
import os
import cv2
import photoshop.api as ps
#pip install photoshop_python_api

def hideAllLayersExcept(list1):
	# hide all layers except text
	for l in range(0, doc.ArtLayers.Count, 1):
		layer = doc.ArtLayers[l]
		if layer.name in list1:
			layer.visible = True
			continue
		layer.visible = False
		print(layer.name)

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

#text_of_layer.rasterize(ps.RasterizeType.TEXTCONTENTS)
options = win32com.client.Dispatch('Photoshop.ExportOptionsSaveForWeb')
options.Format = 13   # PNG Format
options.PNG8 = False  # Sets it to PNG-24 bit

#psApp.Application.ActiveDocument.rasterizeAllLayers()


hideAllLayersExcept(set(["rect1","Facts"]))
layer_facts.visible = True
pngfile = f"D:\\projects\\VideoProjects\\Prob_Terminator_scenario\\text.png"
doc.Export(ExportIn=pngfile, ExportAs=2, Options=options)

hideAllLayersExcept(set(["background"]))
pngfile = f"D:\\projects\\VideoProjects\\Prob_Terminator_scenario\\background.png"
doc.Export(ExportIn=pngfile, ExportAs=2, Options=options)

#text_of_layer.contents = "Costum text from Python"
print(text_of_layer.contents)

#doc.ResizeImage(Width=2048,
#                Height=2048,
#                Resolution=300,
#                ResampleMethod=8)
#layerPs = doc.ArtLayers["Name_of_Layer_Here"]
#layerPs.visible = False # Or True if you want to unhide it.
