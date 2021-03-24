MSX2++ emulator

                                                         24/03/2021 madscient

--- What's this?

This is an implementation as technology preview of the idea for MSX2+ video improvements.
It has another VDP (V9958), which can be superimposed over/under the standard VDP.

For software compatibility, the two VDPs are connected to the same I/O port by default, and
also, only the standard VDP will be displayed.
As long as you don't access the added I/O ports, it will work exactly the same as the existing MSX2+.

The added I/O ports are as follows:

    | bit7 | bit6 | bit5 | bit4 | bit3 | bit2 | bit1 | bit0 |
B8H |      |      |   Display   |      |      |      | Port |

Port:
  0: Map both VDPs on same port 98-9BH.
     Data written to ports 98-9BH will be written to both VDPs.
     Reading from port 98-9BH returns the data of the primary VDP.

  1: Map the primary VDP to port 98-9BH and the secondary VDP to port 88-8BH.
     This allows individual read/write access to each VDP.

Display:
  0: Display Primary VDP only.
  1: Display Secondary VDP only.
  2: Superimpose Primary VDP over Secondary VDP.
  3: Superimpose Secondary VDP over Primary VDP.

--- License

This project was forked from yayaMSX2+ (Common Source Code Project)
Original source codes were written by Mr.Takeda and Mr.umaiboux.

http://takeda-toshiya.my.coocan.jp/
http://umaiboux.k-free.net/cscp.html

The copyright belongs to the author, but you can use the source codes
under the GNU GENERAL PUBLIC LICENSE Version 2.

See also LICENSE for more details about the license.

The license statements of original codes are under license.org directory.