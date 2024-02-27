from scipy.io import wavfile
from scipy.fft import fft, fftfreq
from matplotlib import pyplot as plt
from matplotlib.widgets import Slider
import numpy as np
import csv


RANGE = 2000
SAMPLE_RATE = 44100


def read_signal(path: str, ft: str):
    match ft:
        # load .wav file
        case "wav":
            sample_rate, signal = wavfile.read(path)
            spec = fft(signal)
            freq = fftfreq(len(spec), 1/sample_rate)
        # load .csv file
        case "csv":
            with open(path, newline="") as file:
                reader = csv.reader(file, delimiter=",")
                for signal in reader:
                    spec = [float(pt) for pt in signal]
                    break
            spec = fft(signal)
            freq = fftfreq(len(spec), 1/SAMPLE_RATE)
        case _:
            raise Exception("bad filetype")

    # trim negatives
    spec = spec[:len(spec)//2]
    freq = freq[:len(freq)//2]

    return (freq, np.array(np.abs(spec), dtype=float))


# freq, spec = read_signal("./piano2_mono.wav", ft="wav")
# freq, spec = read_signal("./piano1_mono.wav", ft="wav")
# freq, spec = read_signal("./trumpet.csv", ft="csv")
freq, spec = read_signal("./twotrumpetsAB.csv", ft="csv")

ymax = np.max(np.abs(spec)) * 1.2

# plot
fig, ax = plt.subplots()
ax.set_ylim(0, ymax)
ax.grid()
line, = plt.plot(freq, spec)
ax.set_xlabel("Frequecy Hz")

fig.subplots_adjust(bottom=0.25)

window = fig.add_axes((0.25, 0.1, 0.63, 0.03))
window_slider = Slider(
    ax=window,
    label="Frequency range",
    valmin=0,
    valmax=len(freq)-RANGE-1,
    valinit=0,
    orientation="horizontal"
)


def window_update(val):
    val = int(val)
    line.set_xdata(freq[val:val+RANGE])
    line.set_ydata(np.abs(spec[val:val+RANGE]))
    ax.relim()
    ax.autoscale_view()
    ax.set_ylim(0, ymax)
    fig.canvas.draw_idle()


window_slider.on_changed(window_update)
plt.show()
