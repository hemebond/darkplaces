# Notes
#
# -- Setup cross-compile --
# meson setup --cross-file x86_64-w64-mingw32.txt build-mingw
# meson compile -C build-mingw
#
# -- Compiler Dedicated Server --
# meson setup --reconfigure --warnlevel everything --buildtype release build
# meson compile -C build
#

project(
	'darkplaces',
	'c',
	# default_options: 'default_library=static'
)


# sdl2_dep = dependency('sdl2')
# sdl2_main_dep = dependency('sdl2main')  # need sdl2_main_dep only for Windows


xmp_dep = declare_dependency(link_with: static_library('xmp'), extra_files: ['snd_xmp.c'])

src_video_capture = [
	'cap_avi.c',
	'cap_ogg.c',
]

src_menu = [
	'menu.c',
	'mvm_cmds.c',
]

src_snd_common = [
	'snd_main.c',
	'snd_mem.c',
	'snd_mix.c',
	'snd_ogg.c',
	'snd_wav.c',
]

src_snd_xmp = [
	'snd_xmp.c'
]

src_common = [
	'bih.c',
	'crypto.c',
	'cd_shared.c',
	'cl_cmd.c',
	'cl_collision.c',
	'cl_demo.c',
	'cl_ents.c',
	'cl_ents4.c',
	'cl_ents5.c',
	'cl_ents_nq.c',
	'cl_ents_qw.c',
	'cl_input.c',
	'cl_main.c',
	'cl_parse.c',
	'cl_particles.c',
	'cl_screen.c',
	'cl_video.c',
	'cl_video_libavw.c',
	'clvm_cmds.c',
	'cmd.c',
	'collision.c',
	'com_crc16.c',
	'com_ents.c',
	'com_ents4.c',
	'com_game.c',
	'com_infostring.c',
	'com_msg.c',
	'common.c',
	'console.c',
	'csprogs.c',
	'curves.c',
	'cvar.c',
	'dpvsimpledecode.c',
	'filematch.c',
	'fractalnoise.c',
	'fs.c',
	'ft2.c',
	'utf8lib.c',
	'gl_backend.c',
	'gl_draw.c',
	'gl_rmain.c',
	'gl_rsurf.c',
	'gl_textures.c',
	'hmac.c',
	'host.c',
	'image.c',
	'image_png.c',
	'jpeg.c',
	'keys.c',
	'lhnet.c',
	'libcurl.c',
	'mathlib.c',
	'matrixlib.c',
	'mdfour.c',
	'meshqueue.c',
	'mod_skeletal_animatevertices_sse.c',
	'mod_skeletal_animatevertices_generic.c',
	'model_alias.c',
	'model_brush.c',
	'model_shared.c',
	'model_sprite.c',
	'netconn.c',
	'palette.c',
	'phys.c',
	'polygon.c',
	'portals.c',
	'protocol.c',
	'prvm_cmds.c',
	'prvm_edict.c',
	'prvm_exec.c',
	'r_explosion.c',
	'r_lightning.c',
	'r_modules.c',
	'r_shadow.c',
	'r_sky.c',
	'r_sprites.c',
	'r_stats.c',
	'sbar.c',
	'sv_ccmds.c',
	'sv_demo.c',
	'sv_ents.c',
	'sv_ents4.c',
	'sv_ents5.c',
	'sv_ents_csqc.c',
	'sv_ents_nq.c',
	'sv_main.c',
	'sv_move.c',
	'sv_phys.c',
	'sv_save.c',
	'sv_send.c',
	'sv_user.c',
	'svbsp.c',
	'svvm_cmds.c',
	'sys_shared.c',
	'taskqueue.c',
	'vid_shared.c',
	'view.c',
	'wad.c',
	'world.c',
	'zone.c',
]

src_snd_null = [
	'snd_null.c'
]

src_sv = [
	'builddate.c',
	'sys_null.c',
	'vid_null.c',
	'thread_null.c',
] + src_snd_null \
  + src_common

src_sdl = [
	'builddate.c',
	'sys_sdl.c',
	'vid_sdl.c',
	'thread_sdl.c',
] + src_menu \
  + src_snd_common \
  + src_snd_xmp \
  + ['snd_sdl.c'] \
  + src_video_capture \
  + src_common

# LDFLAGS_UNIXCOMMON=-lm $(LIB_ODE) $(LIB_Z) $(LIB_JPEG) $(LIB_CRYPTO) $(LIB_CRYPTO_RIJNDAEL) $(LIB_SND_XMP)
# LDFLAGS_UNIXSV_PRELOAD=-lz -ljpeg -lpng -lcurl

cc = meson.get_compiler('c')
mathlib_dep = cc.find_library('m', required: false)

xmp_lib = cc.find_library('xmp', required: false)
if xmp_lib.found()
	xmp_flags = ['-DUSEXMP', '-DLINK_TO_LIBXMP']
else
	xmp_flags = []
endif

linux_deps = [
	mathlib_dep,
	cc.find_library('rt'),
	cc.find_library('dl'),
	cc.find_library('z'),
	cc.find_library('jpeg'),
	cc.find_library('png'),
	cc.find_library('curl'),
]

executable(
	'darkplaces-server',
	sources: src_sv,
	dependencies: [linux_deps],
	c_args: '-rdynamic',
)

executable(
	'darkplaces-sdl',
	sources: src_sdl,
	dependencies: [
		cc.find_library('m'),
		cc.find_library('z'),
		cc.find_library('jpeg'),
		cc.find_library('png'),
		cc.find_library('ogg'),
		cc.find_library('theora'),
		cc.find_library('vorbis'),
		cc.find_library('vorbisenc'),
		cc.find_library('vorbisfile'),
		cc.find_library('curl'),
		cc.find_library('X11'),
		cc.find_library('rt'),
		cc.find_library('dl'),
		xmp_lib,
		dependency('sdl2'),
	],
	c_args: ['-DCONFIG_MENU', '-DCONFIG_VIDEO_CAPTURE', '-DLINK_TO_ZLIB', '-DLINK_TO_LIBJPEG', xmp_flags]
)
