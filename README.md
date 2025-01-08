# Rovenhell
Rovenhell is a project aimed at creating a server-authoritative MMO game server architecture using distributed dedicated servers.  
However, it was discontinued due to architectural limitations discovered during implementation. 

The project is NOT intended to be a production-ready tool; its primary goal was to help me learn network programming and test ideas, exploring why certain concepts are rarely used in real-world MMOs. 

Instead of relying on Unreal's replication system, Rovenhell uses custom-written netcode to synchronize approximately 200 players per dedicated server. However, the player base does not scale linearly with distribution, which was the core reason for discontinuing the project.  
For more information, visit [here](https://gamesmith.tistory.com/261).  
  
## Showcase
![playerTest](https://github.com/hagukin/Rovenhell_UE/assets/63915665/f5c3cbeb-9969-4be6-9c48-fd02c0a80829)
## Architecture Overview
![BriefArchitectureOverview](https://github.com/hagukin/Rovenhell_UE/assets/63915665/f63258fb-0c64-4594-951b-eaaa3514469a)
## Scale-out Strategy (Unimplemented)
![potentialScaleOutStrategy](https://github.com/hagukin/Rovenhell_UE/assets/63915665/491e065a-429c-40d6-b1aa-a9cf3c2eac59)
![scaleout2](https://github.com/hagukin/Rovenhell_UE/assets/63915665/4e7d5369-ab78-4e96-b71e-66647a8767ba)

