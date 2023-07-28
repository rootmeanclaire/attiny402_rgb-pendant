Build Errors
------------

The toolchain for the new ATtiny 0-series and 1-series is not up to date in a
lot of package repositories. Most of these errors have been fixed [upstream][1],
but I have included minimal versions of the patches here.

```
```

Fixed with `patches/ldscript.patch`.
Apply this to `/usr/avr/lib/ldscripts/avrxmega3.xn`.
The exact path may be slightly different for you.
This patch is from [the forums][2], not upstream.

```
/usr/avr/include/avr/io.h:583:6: warning: #warning "device type not defined" [-Wcpp]
  583 | #    warning "device type not defined"
```

Fixed with `patches/devdef.patch`.
Apply this to `/usr/avr/include/avr/io.h`.
The exact path may be slightly different for you.

```
main.c:115:9: error: 'MCUCR' undeclared (first use in this function)
  115 |         set_sleep_mode(SLEEP_MODE_STANDBY);
      |         ^~~~~~~~~~~~~~
```

Fixed with `patches/sleep.patch`.
Apply this to `/usr/avr/include/avr/sleep.h`
The exact path may be slightly different for you.

[1]: https://github.com/avrdudes/avr-libc/
[2]: https://www.avrfreaks.net/s/topic/a5C3l000000UaSiEAK/t153954?comment=P-1378383
