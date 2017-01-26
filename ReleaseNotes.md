# 01.06.2017 
Initial release. 

# 01.27.2017 
These builds incorporated a few bug-fixes introduced in the main `Urho3D` repository as well as new features and upgrades of existing 3rd party libraries. Most notably: 
## Upgraded AngelScript from 2.30.2 to 2.31.2. 
## Completely removed support for touch controls and touch input. (Because `Windows` and `Linux` are the only two target platforms that are officially supported, and touch controls are more popular on mobile devices. Also the code base would be simplified a lot.) 
## Gotten rid of legacy and/or unpopular file formats to reduce the size of the AssetImporter tool. (around 5 MiB was cut due to the dropping of a lot of file formats) 
## New editor features: Show object's origin and paint selection. 
