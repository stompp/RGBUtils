import yaml


def can_be_digital(rgb):
    is_digital = True
    for c in rgb:
        if not(c == 255 or c == 0):
            is_digital = False
            break
    return is_digital


f = open("colors.json","r")
out = open("colors_defines.h","w")
p = yaml.safe_load(f.read())
out.write("#ifndef COLORS_DEFINES_H_\n#define COLORS_DEFINES_H_\n\n")
lines = []
digital_lines = []

for k in p.keys():
    rgb = p[k]['rgb']
    lines.append("#define COLOR_" + k.upper() + " {},{},{}".format(rgb[0],rgb[1],rgb[2]) +"\n")

    if can_be_digital(rgb):
        digital_lines.append("#define DIGITAL_COLOR_"+k.upper() + " {},{},{}".format(int(rgb[0]/255),int(rgb[1]/255),int(rgb[2]/255)) +"\n")

digital_lines.append("#define DIGITAL_COLOR_GREEN 0,1,0\n")
out.write("//PWM\n\n")
out.writelines(lines)
out.write("\n//DIGITAL\n\n")
out.writelines(digital_lines)

out.write("\n#endif")
f.close()
out.close()




