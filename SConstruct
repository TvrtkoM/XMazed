duplicate = 0

VariantDir("build", "src", duplicate)
VariantDir("bin", ".", duplicate)

env = Environment()
env.ParseConfig("pkg-config --cflags --libs gtkmm-2.4")
env.ParseConfig("pkg-config --cflags --libs cairomm-1.0")
Export("env")

SConscript("build/SConscript", exports = "env");

env.Append(LIBS = ['xmwin'], CPPPATH = ['src'], 
		LIBPATH = ['build'], CFLAGS=['-ggdb'])

env.Program("bin/xmazed", "bin/xmazed.cc")
