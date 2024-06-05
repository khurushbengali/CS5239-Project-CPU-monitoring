#!/usr/bin/python3

import matplotlib.pyplot as plt


def main():
	# TODO: for each compression level in {0, 1, ..., 9}:
	#			fill real_time [ms]
	#			fill archive_size [bytes]
	level                = list(range(10))
	real_time_big        = [54, 95, 134, 147, 137, 214, 341, 440, 501, 489]
	archive_size_big     = [6488666, 2848211, 2724471, 2609122, 2522250, 2428106, 2385237, 2376869, 2372127, 2372107]
	real_time_marbles    = [41, 124, 124, 91, 142, 160, 168, 173, 179, 187]
	archive_size_marbles = [4264316, 2582921, 2535089, 2505860, 2529535, 2516390, 2512654, 2512067, 2511624, 2511497]
	real_time_ray        = [7, 48, 19, 30, 59, 37, 91, 115, 173, 188]
	archive_size_ray     = [1440054, 331089, 310084, 285593, 255405, 240715, 231578, 229028, 227360, 227226]
	real_time_all        = [42, 192, 235, 261, 298, 410, 527, 627, 805, 825]
	archive_size_all     = [12193036, 5762221, 5569644, 5569644, 5307190, 5185211, 5129469, 5117964, 5111111, 5110830]

	archive_size_big     = [it / 1024 for it in archive_size_big]
	archive_size_marbles = [it / 1024 for it in archive_size_marbles]
	archive_size_ray     = [it / 1024 for it in archive_size_ray]
	archive_size_all     = [it / 1024 for it in archive_size_all]

	# plot data
	fix, axs = plt.subplots(2)

	axs[0].plot(level, real_time_big,        label='Big Text')
	axs[1].plot(level, archive_size_big,     label='Big Text')
	axs[0].plot(level, real_time_marbles,    label='Marbles Bitmap')
	axs[1].plot(level, archive_size_marbles, label='Marbles Bitmap')
	axs[0].plot(level, real_time_ray,        label='Ray Bitmap')
	axs[1].plot(level, archive_size_ray,     label='Ray Bitmap')
	axs[0].plot(level, real_time_all,        label='All Three Files')
	axs[1].plot(level, archive_size_all,     label='All Three Files')

	axs[0].set_xticks(level)
	axs[1].set_xticks(level)

	axs[0].grid(True, which='both')
	axs[1].grid(True, which='both')

	axs[0].set_xlabel('Compression level')
	axs[1].set_xlabel('Compression level')

	axs[0].set_ylabel('Time [ms]')
	axs[1].set_ylabel('Archive size [KiloByte]')
	
	axs[0].legend()
	axs[1].legend()

	plt.show()


if __name__ == '__main__':
	main()