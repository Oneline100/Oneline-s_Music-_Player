# Oneline Music Display
An AI Music Display for Spotify and Gemini with a volume wheel.  

## What can it do?
- Control your playlist on Spotify or other apps
- Gemini Desk Assistant
- Timer
- More coming soon
<img width="600" height="700" alt="image" src="https://github.com/user-attachments/assets/f2c72b69-f68a-499d-9a88-e8495f5cd878" />

## What has been done?
- [x] CAD
- [x] Wiring in KiCAD
- [ ] Firmware
- [ ] Printing Case
- [ ] Getting the parts
- [ ] Wiring and Building
- [ ] Testing
- [ ] Done!!! :tada:

## CAD
<img width="527" height="478" alt="{977855EE-B161-45B3-8307-2B214859040B}" src="https://github.com/user-attachments/assets/d2c55be2-7aad-4728-94ef-2cd01ae6743a" />

## Wiring
<img width="982" height="469" alt="{56BE60A5-97ED-49B2-9833-71416ECB8D9A}" src="https://github.com/user-attachments/assets/75c6ce28-b8ce-4983-9bc0-ff4d0a5037ce" />

## BOM
| Name | Purpose | Cost (USD) | Qty | Total (USD) | Funded |  | Distributor |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **Solder Wick Braid with Flux** | Soldering | 4.48 | 1 | 4.48 | ❌ No | [Link](https://www.amazon.com/gp/product/B0F4PNVHCL) | Amazon |
| **Lead Free Solder Wire** | Soldering | 7.99 | 1 | 7.99 | ❌ No | [Link](https://www.amazon.com/gp/product/B0F8W39725) | Amazon |
| **Wire Strippers** | Wiring | 5.99 | 1 | 5.99 | ❌ No | [Link](https://www.amazon.com/gp/product/B0FRZ7791X) | Amazon |
| **Heat Shrink Tubing Kit** | Wiring | 7.99 | 1 | 5.09 | ✅ Yes | [Link](https://www.amazon.com/Ginsco-580-pcs-Assorted-Sleeving/dp/B01MFA3OFA) | Amazon |
| **Super Soft Copper Wire Box** | Wiring | 5.86 | 1 | 5.86 | ✅ Yes | [Link](https://www.aliexpress.us/item/3256806164419666.html) | AliExpress |
| **1.8" TFT display** | Display | 3.89 | 1 | 3.89 | ✅ Yes | [Link](https://www.aliexpress.us/item/3256804553736450.html) | AliExpress |
| **LOLIN C3 mini** | Computing | 5.47 | 1 | 5.47 | ✅ Yes | [Link](https://www.aliexpress.us/item/3256804553736450.html) | AliExpress |
| **GRAND TOTAL** | | | | **41.67** | | | |

## Firmware
> [!NOTE]
> This code has not been tested, so errors may occour.

This is 2 part firmware solution that will both control music and get AI responses for the computer that show on the screen of the music player.
One is python for the Gemini AI api and Spotify will be contorled by the music player. 
