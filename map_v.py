from PIL import Image


if __name__== "__main__":
    text = open("map.txt", "r")
    image = Image.new("RGB", (800, 2000), "white")

    brown = (139, 69, 19)       #Dirt
    black = (0, 0, 0)           #Ground
    grey = (128, 128, 128)      #Stone
    gold = (255, 215, 0)        #Gold
    yellow = (255, 255, 0)      #BRONZE
    green = (0, 128, 0)         #WOOD
    blue = (0, 0, 255)          #SNOW
    red = (255, 0, 0)           #LAVA
    silver = (192, 192, 192)    #SILVER
    orange = (255, 165, 0)      #COPPER
    purple = (128, 0, 128)
    pink = (255, 192, 203)
    

    for i, line in enumerate(text):
        for j, ch in enumerate(line):
            if ch == "0":
                continue
            elif ch == "1":
                image.putpixel((j, i), black)
            elif ch == "2":
                image.putpixel((j, i), brown)
            elif ch == "3":
                pass
            elif ch == "4":
                pass
            elif ch == "5":
                pass
            elif ch == "6":
                pass
            elif ch == "7":
                pass
            elif ch == "8":
                pass
            elif ch == "9":
                pass
            elif ch == "0":
                pass
            elif ch == "a":
                pass
            elif ch == "b":
                pass





    image.save("map.png")

    