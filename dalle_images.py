import openai

response = openai.Image.create(prompt = "6 small images contain a pixel sprite useable for 2d animation of the same character in different states of walking", n = 4, size = "1024x1024 ")
image_url - response
openai.