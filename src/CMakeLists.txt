add_subdirectory(wads)

add_executable(rusdoom
    am_map.c        am_map.h
                    d_englsh.h
                    d_event.h
    d_main.c        d_main.h
    d_net.c         d_net.h
                    d_player.h
                    d_think.h
                    d_ticcmd.h
    dmx.c           dmx.h
                    doomdata.h
                    doomdef.h
                    doomstat.h
                    doomtype.h
                    dpmiapi.h
    dutils.c        dutils.h
    f_finale.c      f_finale.h
    f_wipe.c        f_wipe.h
                    fx_man.h
    g_game.c        g_game.h
    hu_lib.c        hu_lib.h
    hu_stuff.c      hu_stuff.h
    i_cyber.c
    i_ibm.c
    i_main.c
    i_sound.c       i_sound.h
                    i_system.h
                    i_video.h
    info.c          info.h
                    jn.h
                    limits.h
    m_menu.c        m_menu.h
    m_misc.c        m_misc.h
    mus2mid.c       mus2mid.h
                    music.h
    p_ceilng.c
    p_doors.c
    p_enemy.c
    p_floor.c
    p_inter.c       p_inter.h
    p_lights.c
                    p_local.h
    p_map.c
    p_maputl.c
    p_mobj.c        p_mobj.h
    p_plats.c
    p_pspr.c        p_pspr.h
    p_saveg.c       p_saveg.h
    p_setup.c       p_setup.h
    p_sight.c
    p_spec.c        p_spec.h
    p_switch.c
    p_telept.c
    p_tick.c        p_tick.h
    p_user.c
    r_bmaps.c       r_bmaps.h
    r_bsp.c         r_bsp.h
    r_data.c        r_data.h
                    r_defs.h
    r_draw.c        r_draw.h
                    r_local.h
    r_main.c        r_main.h
    r_plane.c       r_plane.h
    r_segs.c        r_segs.h
                    r_state.h
    r_things.c      r_things.h
    rd_lang.c       rd_lang.h
    s_sound.c       s_sound.h
                    sndcards.h
    sounds.c        sounds.h
    st_lib.c        st_lib.h
    st_stuff.c      st_stuff.h
    tables.c        tables.h
                    task_man.h
    usrhooks.c      usrhooks.h
    v_trans.c       v_trans.h
    v_video.c       v_video.h
    w_wad.c         w_wad.h
    wi_stuff.c      wi_stuff.h
    z_zone.c        z_zone.h
)

target_link_libraries(rusdoom PRIVATE
    audio_lib
)

set_target_properties(rusdoom PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED ON
    OUTPUT_NAME "RUSDOOM"
    SUFFIX ".EXE"
)

target_compile_options(rusdoom PRIVATE
    -wx # maximum warnings
    -wcd=303 # ignore "303: Parameter * has been defined, but not referenced" warning
    -j # treat char as signed
    -ei # use 4 bytes for enum
    -zastd=c99 # use C99 standard (Cmake fails to add this with target properties)
)

if(NOT RD_GIT_NO_HASH)
    add_dependencies(rusdoom revision_check)
endif()

install(TARGETS rusdoom RUNTIME
    DESTINATION .
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)
