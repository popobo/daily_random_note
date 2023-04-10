from psd_tools import PSDImage

psd = PSDImage.open('example.psd')
psd.composite().save('example.png')

for layer in psd:
    print(layer)
    layer_image = layer.composite()
    layer_image.save('%s.png' % layer.name)