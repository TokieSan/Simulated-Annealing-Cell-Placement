import os
import glob
from PIL import Image, ImageDraw, ImageFont
import imageio
import re

def read_step_file(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()
    
    grid = []
    cost = None
    
    for line in lines:

        if line.startswith("COST:"):
            cost = int(line.split()[1])
        else:
            row = line.strip().split()
            grid.append([int(x) for x in row])
    
    return grid, cost

def create_image(grid, cost, font):
    cell_size = 50
    margin = 20
    rows, cols = len(grid), len(grid[0])
    
    img_width = cols * cell_size + 2 * margin
    img_height = rows * cell_size + 2 * margin + 50
    
    image = Image.new('RGB', (img_width, img_height), color='white')
    draw = ImageDraw.Draw(image)
    
    for i, row in enumerate(grid):
        for j, val in enumerate(row):
            x0 = margin + j * cell_size
            y0 = margin + i * cell_size
            x1 = x0 + cell_size
            y1 = y0 + cell_size
            draw.rectangle([x0, y0, x1, y1], outline='black')
            text = str(val) if val != -1 else '--'
            bbox = draw.textbbox((0, 0), text, font=font)
            w, h = bbox[2] - bbox[0], bbox[3] - bbox[1]
            draw.text(((x0 + x1) / 2 - w / 2, (y0 + y1) / 2 - h / 2), text, fill='black', font=font)
    
    cost_text = f"COST: {cost}"
    draw.text((margin, img_height - 40), cost_text, fill='black', font=font)
    return image

def sort_files_numerically(files):
    def extract_number(file_name):
        match = re.search(r'_(\d+)\.txt$', file_name)
        return int(match.group(1)) if match else -1

    return sorted(files, key=extract_number)

def create_gif_from_steps(step_files, output_gif, step_interval = 100):
    font = ImageFont.load_default()
    
    images = []
    for i, step_file in enumerate(step_files):
        if step_interval == -1 or i % step_interval == 0:  # skip frames to reduce total number of images
            grid, cost = read_step_file(step_file)
            image = create_image(grid, cost, font)
            images.append(image)

    images[0].save(output_gif, save_all=True, append_images=images[1:], duration=500, loop=0)


step_files = glob.glob('step_*.txt')
step_files = sort_files_numerically(step_files)

step_interval = -1 # no skipping

if len(step_files) > 200:
    step_interval = len(step_files) // 200

if len(os.sys.argv) > 1:
    step_interval = int(os.sys.argv[1])
create_gif_from_steps(step_files, 'simulation_steps.gif', step_interval)

print("GIF created successfully.")
