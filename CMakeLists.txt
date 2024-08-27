cmake_minimum_required(VERSION 3.25)

# Windows RunTime Library
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Set universal binary build on MacOS
set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")

# Set macOS target deployment version
set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0")

# Define project
project(mrta
    VERSION 0.0.1
    LANGUAGES C CXX)

# Xcode 15 linker workaround
# If you are using Link Time Optimisation (LTO), the new linker introduced in Xcode 15 may produce a broken binary.
# As a workaround, add either '-Wl,-weak_reference_mismatches,weak' or '-Wl,-ld_classic' to your linker flags.
# Once you've selected a workaround, you can add JUCE_SILENCE_XCODE_15_LINKER_WARNING to your preprocessor definitions to silence this warning.
if(APPLE)
    set(xcode_15_linker -Wl,-ld_classic)
endif()

# Some windows specific defines
if (WIN32)
    set(windows_defines _USE_MATH_DEFINES WIN32_LEAN_AND_MEAN)
endif()

# Add JUCE
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/JUCE)

# Add MRTA module
juce_add_module(${CMAKE_CURRENT_SOURCE_DIR}/modules/mrta_utils)

# General variables
# You should change this to something more interesting
set(company_name "Modern Real-Time Audio")
set(company_copyright "Modern Real-Time Audio Oy")
set(company_code "Mrta")
set(company_bundle_id "com.ModernRealTimeAudio")

# This function adds a JUCE plugin target to this project
# Arguments:
#   - TARGET: The name of your plugin target, this allows you to identify which of your plugins will be built.
#   - VERSION: Version on your plugin.
#   - PLUGIN_NAME: The name which will be displayed on the DAW.
#   - PROD_NAME: Internal product name, cannot contain whitespace.
#   - PROD_CODE: 4 letter code unique identifier to your plugin, at least 1 capitalized letter.
#   - SYNTH: Set to true if your plugin is a synth, false otherwise.
#   - SOURCES: A list of all the source files of you plugin.
#   - INCLUDE_DIRS: A list of the include directories required by your sources.
function(add_plugin target)
    # parse input args
    set(one_value_args TARGET VERSION PLUGIN_NAME PROD_NAME PROD_CODE SYNTH)
    set(multi_value_args SOURCES INCLUDE_DIRS)
    cmake_parse_arguments(AP "" "${one_value_args}" "${multi_value_args}" ${ARGN})

    # info and debug
    message(STATUS "Adding JUCE plugin target: ${target}")
    message(STATUS "  PLUGIN_NAME: ${AP_PLUGIN_NAME}")
    message(STATUS "  VERSION: ${AP_VERSION}")
    message(STATUS "  PROD_NAME: ${AP_PROD_NAME}")
    message(STATUS "  PROD_CODE: ${AP_PROD_CODE}")
    message(STATUS "  SYNTH: ${AP_SYNTH}")
    ## Too verbose, uncomment just if you need to debug...
    #message(STATUS "  SOURCES: ${AP_SOURCES}")
    #message(STATUS "  INCLUDE_DIRS: ${AP_INCLUDE_DIRS}")

    # Add juce plugin target
    juce_add_plugin(${target}
        PRODUCT_NAME ${AP_PROD_NAME}
        VERSION ${AP_VERSION}
        MICROPHONE_PERMISSION_ENABLED TRUE
        COMPANY_COPYRIGHT ${company_copyright}
        COMPANY_NAME ${company_name}
        FORMATS Standalone VST3 AU Standalone
        PLUGIN_NAME ${AP_PLUGIN_NAME}
        PLUGIN_MANUFACTURER_CODE ${company_code}
        PLUGIN_CODE ${AP_PROD_CODE}
        BUNDLE_ID ${company_bundle_id}.${AP_PROD_NAME}
        IS_SYNTH ${AP_SYNTH}
        NEEDS_MIDI_INPUT ${AP_SYNTH}
        NEEDS_MIDI_OUTPUT FALSE
        COPY_PLUGIN_AFTER_BUILD TRUE)
    juce_generate_juce_header(${target})

    target_sources(${target}
        PRIVATE
            ${AP_SOURCES})

    target_include_directories(${target}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/asiosdk/common
            ${AP_INCLUDE_DIRS})

    target_compile_features(${target}
        PUBLIC
            cxx_std_17)

    target_compile_definitions(${target}
        PRIVATE
            JUCE_ASIO=1 JUCE_DIRECTSOUND=0 JUCE_USE_FLAC=0 JUCE_USE_OGGVORBIS=0
            JUCE_USE_WINDOWS_MEDIA_FORMAT=0 JUCE_WEB_BROWSER=0
            JUCE_VST3_CAN_REPLACE_VST2=0 JUCE_SILENCE_XCODE_15_LINKER_WARNING=1
            ${windows_defines})

    target_link_libraries(${target}
        PRIVATE
            juce::juce_audio_utils
            juce::juce_dsp
            mrta_utils
        PUBLIC
            ${xcode_15_linker}
            juce::juce_recommended_config_flags
            juce::juce_recommended_lto_flags)

endfunction(add_plugin)


## Plugin projects

set(dsp_source ${CMAKE_CURRENT_SOURCE_DIR}/projects/DSP)
set(gui_source ${CMAKE_CURRENT_SOURCE_DIR}/projects/GUI)

# add example project
set(mfrtaa_source ${CMAKE_CURRENT_SOURCE_DIR}/projects/MyFirstRealTimeAudioApp)

add_plugin(mfrtaa
    VERSION 0.1.0
    PLUGIN_NAME "My First RT Audio App"
    PROD_NAME MyFirstRTAudioApp
    PROD_CODE Rtap
    SYNTH FALSE
    SOURCES
        ${mfrtaa_source}/PluginEditor.cpp
        ${mfrtaa_source}/PluginProcessor.cpp
    INCLUDE_DIRS
        ${mfrtaa_source})


# ring mod project
set(ringmod_source ${CMAKE_CURRENT_SOURCE_DIR}/projects/RingModulator)

add_plugin(ringmod
    VERSION 0.1.0
    PLUGIN_NAME "Ring Modulator"
    PROD_NAME RingModulator
    PROD_CODE Rmod
    SYNTH FALSE
    SOURCES
        ${ringmod_source}/PluginEditor.cpp
        ${ringmod_source}/PluginProcessor.cpp
        ${dsp_source}/RingMod.cpp
    INCLUDE_DIRS
        ${dsp_source}
        ${ringmod_source})

# parametric eq project
set(parameq_source ${CMAKE_CURRENT_SOURCE_DIR}/projects/ParametricEQ)

add_plugin(parameq
    VERSION 0.1.0
    PLUGIN_NAME "Parametric EQ"
    PROD_NAME ParametricEQ
    PROD_CODE Pmeq
    SYNTH FALSE
    SOURCES
        ${parameq_source}/PluginEditor.cpp
        ${parameq_source}/PluginProcessor.cpp
        ${dsp_source}/Biquad.cpp
        ${dsp_source}/ParametricEqualizer.cpp
        ${gui_source}/MrtaLAF.cpp
    INCLUDE_DIRS
        ${gui_source}
        ${dsp_source}
        ${ringmod_source})

# flanger project
set(flanger_source ${CMAKE_CURRENT_SOURCE_DIR}/projects/Flanger)

add_plugin(flanger
    VERSION 0.1.0
    PLUGIN_NAME "Flanger"
    PROD_NAME Flanger
    PROD_CODE Flng
    SYNTH FALSE
    SOURCES
        ${flanger_source}/PluginEditor.cpp
        ${flanger_source}/PluginProcessor.cpp
        ${dsp_source}/DelayLine.cpp
        ${dsp_source}/Flanger.cpp
    INCLUDE_DIRS
        ${dsp_source}
        ${flanger_source})

# Velvet Reverb Project
set(velvetreverb_source ${CMAKE_CURRENT_SOURCE_DIR}/projects/VelvetReverb)

add_plugin(velvetreverb
    VERSION 0.1.0
    PLUGIN_NAME "Velvet Reverb"
    PROD_NAME Velvet Reverb
    PROD_CODE Vtrb
    SYNTH FALSE
    SOURCES
        ${velvetreverb_source}/PluginEditor.cpp
        ${velvetreverb_source}/PluginProcessor.cpp
        ${dsp_source}/MultiTapDelayLine.cpp
        ${dsp_source}/VelvetReverb.cpp
    INCLUDE_DIRS
        ${dsp_source}
        ${velvetreverb_source})

# delay project
set(delay_source ${CMAKE_CURRENT_SOURCE_DIR}/projects/Delay)

add_plugin(delay
    VERSION 0.1.0
    PLUGIN_NAME "Delay"
    PROD_NAME Delay
    PROD_CODE Dlay
    SYNTH FALSE
    SOURCES
        ${delay_source}/PluginEditor.cpp
        ${delay_source}/PluginProcessor.cpp
        ${dsp_source}/DelayLine.cpp
        ${dsp_source}/Delay.cpp
        ${dsp_source}/Biquad.cpp
        ${dsp_source}/ParametricEqualizer.cpp
        ${dsp_source}/Meter.cpp
        ${gui_source}/MeterComponent.cpp
    INCLUDE_DIRS
        ${dsp_source}
        ${gui_source}
        ${delay_source})

# osc project
set(osc_source ${CMAKE_CURRENT_SOURCE_DIR}/projects/Oscillators)

add_plugin(osc
    VERSION 0.1.0
    PLUGIN_NAME "Oscillators"
    PROD_NAME Oscillators
    PROD_CODE Oscs
    SYNTH FALSE
    SOURCES
        ${osc_source}/PluginEditor.cpp
        ${osc_source}/PluginProcessor.cpp
        ${dsp_source}/Oscillator.cpp
    INCLUDE_DIRS
        ${dsp_source}
        ${osc_source})

# midi handling project
set(midi_source ${CMAKE_CURRENT_SOURCE_DIR}/projects/MidiHandler)

add_plugin(midi
    VERSION 0.1.0
    PLUGIN_NAME "Midi Handler"
    PROD_NAME MidiHandler
    PROD_CODE Mdhd
    SYNTH TRUE
    SOURCES
        ${midi_source}/PluginEditor.cpp
        ${midi_source}/PluginProcessor.cpp
        ${dsp_source}/SynthVoice.cpp
        ${dsp_source}/Oscillator.cpp
    INCLUDE_DIRS
        ${dsp_source}
        ${midi_source})

# envelope generator project
set(envgen_source ${CMAKE_CURRENT_SOURCE_DIR}/projects/EnvelopeGenerator)

add_plugin(envgen
    VERSION 0.1.0
    PLUGIN_NAME "Envelope Generator"
    PROD_NAME EnvelopeGenerator
    PROD_CODE Egen
    SYNTH FALSE
    SOURCES
        ${envgen_source}/PluginEditor.cpp
        ${envgen_source}/PluginProcessor.cpp
        ${dsp_source}/EnvelopeGenerator.cpp
    INCLUDE_DIRS
        ${dsp_source}
        ${envgen_source})

# state variable filter project
set(svf_source ${CMAKE_CURRENT_SOURCE_DIR}/projects/StateVariableFilter)

add_plugin(svf
    VERSION 0.1.0
    PLUGIN_NAME "State Variable Filter"
    PROD_NAME StateVariableFilter
    PROD_CODE Svff
    SYNTH FALSE
    SOURCES
        ${svf_source}/PluginEditor.cpp
        ${svf_source}/PluginProcessor.cpp
        ${dsp_source}/StateVariableFilter.cpp
        ${dsp_source}/Oscillator.cpp
    INCLUDE_DIRS
        ${dsp_source}
        ${svf_source})

# Dark Velvet Reverb Project
set(darkvelvetreverb_source ${CMAKE_CURRENT_SOURCE_DIR}/projects/DarkVelvetReverb)

add_plugin(darkvelvetreverb
    VERSION 0.1.0
    PLUGIN_NAME "Dark Velvet Reverb"
    PROD_NAME Dark Velvet Reverb
    PROD_CODE dvnr
    SYNTH FALSE
    SOURCES
        ${darkvelvetreverb_source}/PluginEditor.cpp
        ${darkvelvetreverb_source}/PluginProcessor.cpp
        ${dsp_source}/MultiTapDelayLine.cpp
        ${dsp_source}/DarkVelvetReverb.cpp
    INCLUDE_DIRS
        ${dsp_source}
        ${darkvelvetreverb_source})

# RING MOD HW
set(rm_source ${CMAKE_CURRENT_SOURCE_DIR}/HW/LiRingMod)

add_plugin(rm
    VERSION 0.1.0
    PLUGIN_NAME "rm"
    PROD_NAME rm
    PROD_CODE rmhw
    SYNTH FALSE
    SOURCES
        ${rm_source}/PluginEditor.cpp
        ${rm_source}/PluginProcessor.cpp
    INCLUDE_DIRS
        ${rm_source})

# DELAY HW
set(fbDelay_source ${CMAKE_CURRENT_SOURCE_DIR}/HW/KallinenDelay)

add_plugin(fbDelay
    VERSION 0.1.0
    PLUGIN_NAME "FB delay"
    PROD_NAME fbDelay
    PROD_CODE fbdl
    SYNTH FALSE
    SOURCES
        ${fbDelay_source}/PluginEditor.cpp
        ${fbDelay_source}/PluginProcessor.cpp
        ${dsp_source}/DelayLine.cpp
    INCLUDE_DIRS
        ${fbDelay_source}
        ${dsp_source})