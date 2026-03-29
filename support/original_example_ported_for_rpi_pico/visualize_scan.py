import sys
import math
import re
import tkinter as tk


def parse_input(text: str):
    """
    Parse input in the form:
    [distance,intensity,angle][distance,intensity,angle]...
    Returns a list of (distance, intensity, angle_deg).
    """
    matches = re.findall(
        r'\[\s*([+-]?\d+(?:\.\d+)?)\s*,\s*([+-]?\d+(?:\.\d+)?)\s*,\s*([+-]?\d+(?:\.\d+)?)\s*\]',
        text
    )
    result = []
    for dist_str, inten_str, angle_str in matches:
        distance = float(dist_str)
        intensity = float(inten_str)
        angle_deg = float(angle_str)
        result.append((distance, intensity, angle_deg))
    return result


def intensity_to_gray(intensity: float, min_i: float, max_i: float) -> str:
    """
    Map intensity to a grayscale color.
    """
    if max_i <= min_i:
        value = 200
    else:
        t = (intensity - min_i) / (max_i - min_i)
        t = max(0.0, min(1.0, t))
        value = int(round(t * 255))
    return f"#{value:02x}{value:02x}{value:02x}"


def main():
    text = sys.stdin.read()
    data = parse_input(text)

    if not data:
        print("No valid [distance,intensity,angle] triples found on stdin.", file=sys.stderr)
        sys.exit(1)

    width = 900
    height = 900
    margin = 40
    point_radius = 3

    distances = [d for d, _, _ in data if d >= 0]
    intensities = [i for _, i, _ in data]

    max_distance = max(distances) if distances else 1.0
    if max_distance <= 0:
        max_distance = 1.0

    min_intensity = min(intensities)
    max_intensity = max(intensities)

    cx = width / 2
    cy = height / 2
    usable_radius = min(width, height) / 2 - margin

    root = tk.Tk()
    root.title("Lidar radial view")

    canvas = tk.Canvas(root, width=width, height=height, bg="white")
    canvas.pack()

    # Reference circles
    for frac in (0.25, 0.5, 0.75, 1.0):
        r = usable_radius * frac
        canvas.create_oval(cx - r, cy - r, cx + r, cy + r, outline="#dddddd")

    # Crosshair
    canvas.create_line(cx, margin, cx, height - margin, fill="#eeeeee")
    canvas.create_line(margin, cy, width - margin, cy, fill="#eeeeee")

    for distance, intensity, angle_deg in data:
        angle = math.radians(angle_deg)

        # 0° points up, positive angles go clockwise
        x = cx + math.sin(angle) * (distance / max_distance) * usable_radius
        y = cy - math.cos(angle) * (distance / max_distance) * usable_radius

        gray = intensity_to_gray(intensity, min_intensity, max_intensity)

        # Ray
        canvas.create_line(cx, cy, x, y, fill="#bbbbbb")

        # Endpoint
        canvas.create_oval(
            x - point_radius,
            y - point_radius,
            x + point_radius,
            y + point_radius,
            fill=gray,
            outline=gray,
        )

    # Center marker
    canvas.create_oval(cx - 4, cy - 4, cx + 4, cy + 4, fill="red", outline="red")

    # Info
    canvas.create_text(
        10, 10,
        anchor="nw",
        text=(
            f"points: {len(data)}    "
            f"max distance: {max_distance:.3f} m    "
            f"intensity: {min_intensity:.1f}..{max_intensity:.1f}"
        ),
        fill="black"
    )

    root.mainloop()


if __name__ == "__main__":
    main()
