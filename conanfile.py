from conans import ConanFile

class FlexRigLib(ConanFile):
    name = "FlexRigLib"
    version = "1.0"
    license = "General Attribution License"
    description = "GPU accelorated soft body physics"

    requires = (
        "spdlog/1.8.0",
        "boost/1.75.0",
        "glm/0.9.9.8",
        #"vk-bootstrap/0.4",
        "opencl-headers/2020.06.16"
    )

    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    exports_sources = "*", "!*build/*"

    def build(self):
        cmake.configure()
        cmake.build()

    def imports(self):
        self.copy('*.so*', dst='lib', src='lib')
