# SuirvreX

This repository contains the SuivreX project, a new robotic arm experimental project. 

As an experimental project, it may end up heading in different directions. But the thinking at the outset is that this will involve a C program to talk to a CAN bus network of servo motors. This C program will in turn offer up a WebSocket API that can then be accessed by a JavaScript program to send it commands and receive feedback. The JavaScript program will provide a visual UI of some kind, likely something that eventually gets rendered as WebGL. There will also likely be two robot arms. A mini version that can be used as an input controller (back drivable) and a 10x version that is most definitely not back drivable. May also involve an interface with a controller of some kind, but that will likely come after some kind of inverse kinematics is worked out.

## Key Dependencies - JavaScript interface
As with any modern web application, other JavaScript libraries/dependencies have been used in this project. Most of the time, this is handled via a CDN link (usually JSDelivr) in the Project.html file. In some cases, for performance or other reasons, they may be included directly.
- [TMS WEB Core](https://www.tmssoftware.com/site/tmswebcore.asp) - This is a TMS WEB Core project, after all
- [AdminLTE 4](https://github.com/ColorlibHQ/AdminLTE/tree/v4-dev) - Naturally
- [Home Assistant](https://www.home-assistant.io/) - Need a current Home Assistant server to be of much use
- [Bootstrap](https://getbootstrap.com/) - No introduction needed
- [Tabulator](https://www.tabulator.info) - Fantastic pure JavaScript web data tables
- [Font Awesome](https://www.fontawesome.com) - The very best icons
- [Material Design Icons](https://pictogrammers.com/library/mdi/) - Used throughout Home Assistant
- [Leaflet](https://www.leafletjs.com) - Excellent mapping library
- [OpenStreetMap](https://www.openstreetmap.org) - Mapping tile data
- [Luxon](https://moment.github.io/luxon/#/) - For handling date/time conversions
- [Shoelace](https://shoelace.style/) - Web components, particularly the color picker for lights
- [Peity Vanilla JS](https://github.com/railsjazz/peity_vanilla) - Fast and simple charts
- [Meteocons](https://github.com/basmilius/weather-icons) - Animated Weather Icons by [Bas Milius](https://bas.dev/)
- [SwiperJS](https://swiperjs.com) - Currently works well with v10
- [FlatPickr](https://flatpickr.js.org) - Main UI date pickers
- [Vanilla Lazy Load](https://github.com/verlok/vanilla-lazyload) - So placeholders can be used
- [FlagPack](https://github.com/jackiboy/flagpack) - For displaying citizenship flags
- [PanZoom](https://github.com/timmywil/panzoom) - Used when viewing photos/posters/backgrounds
- [HackTimer](https://github.com/turuslan/HackTimer) - Bypasses normal JavaScript timer behaviour
- [AudioBuffer-ArrayBuffer-Serializer](https://github.com/suzuito/audiobuffer-arraybuffer-serializer) - Useful for moving audio data around
- [InteractJS](https://interactjs.io/) - Dragging and resizing UI elements of all kinds
- [Simplebar](https://github.com/Grsmto/simplebar) - Used to create the custom hexagonal scrollbars
- [D3](https://d3js.org/) - Used here to draw audio waveforms
- [FileSaver](https://moment.github.io/luxon/#/?id=luxon) - For downloading HexaGong projects

## Additional Notes
While this project is currently under active development, feel free to give it a try and post any issues you encounter.  Or start a discussion if you would like to help steer the project in a particular direction.  Early days yet, so a good time to have your voice heard.  As the project unfolds, additional resources will be made available, including platform binaries, more documentation, demos, and so on.

## Repository Information 
[![Count Lines of Code](https://github.com/500Foods/Template/actions/workflows/main.yml/badge.svg)](https://github.com/500Foods/Template/actions/workflows/main.yml)
<!--CLOC-START -->
```
Last updated at 2024-04-08 06:42:16 UTC
-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
Markdown                         1              6              2             54
YAML                             2              8             13             35
HTML                             2              0              0             34
JSON                             1              0              0             34
Pascal                           2             11              2             29
JavaScript                       1              3              0             25
Delphi Form                      1              0              0             13
CSS                              1              1              0              2
-------------------------------------------------------------------------------
SUM:                            11             29             17            226
-------------------------------------------------------------------------------
7 Files (without source code) were skipped
```
<!--CLOC-END-->

## Sponsor / Donate / Support
If you find this work interesting, helpful, or valuable, or that it has saved you time, money, or both, please consider directly supporting these efforts financially via [GitHub Sponsors](https://github.com/sponsors/500Foods) or donating via [Buy Me a Pizza](https://www.buymeacoffee.com/andrewsimard500). Also, check out these other [GitHub Repositories](https://github.com/500Foods?tab=repositories&q=&sort=stargazers) that may interest you.

## More TMS WEB Core and TMS XData Content
If you're interested in other TMS WEB Core and TMS XData content, follow along on 𝕏 at [@WebCoreAndMore](https://x.com/WebCoreAndMore), join our 𝕏 [Web Core and More Community](https://twitter.com/i/communities/1683267402384183296), or check out the [TMS Software Blog](https://www.tmssoftware.com/site/blog.asp).
