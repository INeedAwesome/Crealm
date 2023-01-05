workspace "Crealm"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Crealm"

outputdir = "%{cfg.buildcfg}-x64"

include "Crealm"