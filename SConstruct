# -*- python -*-

def initial_scons_config():
    SConsignFile('build/sconsign.dblite')
    Decider('MD5-timestamp')

def create_options():
    opts = Variables(None, ARGUMENTS)
    opts.Add(BoolVariable('debug', 'Build in debug mode', False))

    return opts

def fill_gcc_env_flags(env):
    env.Append(CCFLAGS=['-Wall', '-Wextra', '-Wpedantic', '-Werror'])
    env.Append(CCFLAGS=['--std=c++17'])

    if env['debug']:
        env.Append(CCFLAGS=['-O0', '-g'])
    else:
        env.Append(CCFLAGS=['-O2'])

def fill_cl_env_flags(env):
    env.Append(CCFLAGS=['/W4', '/WX', '/EHsc'])
    env.Append(CCFLAGS=['--std=c++17'])
    env.Append(LINKFLAGS=['/SUBSYSTEM:CONSOLE'])

    if env['debug']:
        env.Append(CCFLAGS=['/MTd', '/Zi', '/Od'])
        env.Append(LINKFLAGS=['/DEBUG'])
    else:
        env.Append(CCFLAGS=['/MT', '/O2'])

def fill_env_flags(env):
    env.Append(CPPPATH=['#/src'])
    env.Append(CPPPATH=['/usr/local/include']) # TODO configurable
    env.Append(LIBPATH=['/usr/local/lib']) # TODO configurable
    env.Append(LIBPATH=['/home/panov/llvm-project'])

    if env['CC'] == 'cl':
        fill_cl_env_flags(env)
    else:
        fill_gcc_env_flags(env)

def add_special_methods(env):
    boost_libs = [] if env['CC'] == 'cl' else ['boost_unit_test_framework']
    boost_defines = [] if env['CC'] == 'cl' else ['BOOST_TEST_DYN_LINK']

    def run_unit_test(env, target, source):
        import subprocess
        app = str(source[0].abspath)
        if not subprocess.call(app):
            open(str(target[0]),'w').write("PASSED\n")
        else:
            return 1

    def build_and_run_test(env, src, lib, depLibs = []):
        lib_name = str(lib[0]).replace('.', '_')
        test = env.Program(lib_name + "_unit_tests",
                           src,
                           LIBS=[lib] + depLibs + boost_libs,
                           CPPDEFINES=boost_defines)
        env.Command(lib_name + "_test_passed.txt", test, run_unit_test)

    env.AddMethod(build_and_run_test, "BoostTests")

def create_env(opts):
    env = Environment(variables = opts)
    Export('env')
    fill_env_flags(env)
    add_special_methods(env)
    return env

def prepare_options_help(opts, env):
    Help(opts.GenerateHelpText(env))

def build_lib():
    obfuscator_lib = build_dir_contents('src/lib')
    Export('obfuscator_lib')

def build_executable():
    p = SConscript('src/SConscript', variant_dir='build/', duplicate=0)
    env.Install('./', p)




initial_scons_config()

opts = create_options()
env = create_env(opts)

prepare_options_help(opts, env)

build_executable()
