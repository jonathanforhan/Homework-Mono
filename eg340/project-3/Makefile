define task =
mkdir -p build && \
	cmake -B build --preset clang-release && \
	cmake --build build && \
	./build/noisy-tx | tee perf.csv
endef

task: ; $(value task)

.ONESHELL:
