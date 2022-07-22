.. raw:: latex

    \clearpage

.. _binary:


BINARY OUTPUT FILE FORMAT
==========================

The EPANET-MSX system can save the water quality results it computes to
a binary output file. This file can be named and saved to disk using the
MSXsaveoutfile function. The format of the file's contents is described
in :numref:`t-binary` below.

.. tabularcolumns:: |p{8cm}|p{6cm}|

.. _t-binary:
.. table:: Format of the EPANET-MSX binary output file
   :class: longtable

   +-------------------------------------------------+---------------------+
   | **Quantity**                                    | **Size and Type**   |
   +=================================================+=====================+
   | Magic number (516114521)                        | 4-byte integer      |
   +-------------------------------------------------+---------------------+
   | Version number (currently 200000)               | 4-byte integer      |
   +-------------------------------------------------+---------------------+
   | Number of network nodes                         | 4-byte integer      |
   +-------------------------------------------------+---------------------+
   | Number of network links                         | 4-byte integer      |
   +-------------------------------------------------+---------------------+
   | Number of water quality species                 | 4-byte integer      |
   +-------------------------------------------------+---------------------+
   | Reporting time step (seconds)                   | 4-byte integer      |
   +-------------------------------------------------+---------------------+
   || For each water quality species:                ||                    |
   ||   Number of characters in ID name (N)          || 4-byte integer     |
   ||   ID name                                      || N character bytes  |
   ||   Species units                                || 15 character bytes |
   +-------------------------------------------------+---------------------+
   || For each reporting period:                     ||                    |
   ||   For each water quality species:              ||                    |
   ||      For each network node:                    ||                    |
   ||         Nodal water quality result             || 4-byte float       |
   ||                                                ||                    |
   ||   For each water quality species:              ||                    |
   ||      For each network link:                    ||                    |
   ||         Link water quality result              || 4-byte float       |
   +-------------------------------------------------+---------------------+
   | Byte offset where water quality results begin   | 4-byte integer      |
   +-------------------------------------------------+---------------------+
   | Number of reporting periods                     | 4-byte integer      |
   +-------------------------------------------------+---------------------+
   | Error code                                      | 4-byte integer      |
   +-------------------------------------------------+---------------------+
   | Magic number (516114521)                        | 4-byte integer      |
   +-------------------------------------------------+---------------------+

.. 