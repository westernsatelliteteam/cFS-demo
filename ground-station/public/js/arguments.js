const args = {
    cfe_es_noop: { pktid: '0x1806', cmdcode: '0' },
    cfe_evs_noop: { pktid: '0x1801', cmdcode: '0' },
    cfe_sb_noop: { pktid: '0x1803', cmdcode: '0' },
    cfe_tbl_noop: { pktid: '0x1804', cmdcode: '0' },
    cfe_time_noop: { pktid: '0x1805', cmdcode: '0' },

    sb_stats: { pktid: '0x1803', cmdcode: '2' },
    sb_routing: { pktid: '0x1803', cmdcode: '3', string:{ value: "/cf/routing.bin", length: 64 } },
    sb_pipe: { pktid: '0x1803', cmdcode: '7', string:{ value: "/cf/pipe.bin", length: 64 } },
    sb_map: { pktid: '0x1803', cmdcode: '8', string:{ value: "/cf/map.bin", length: 64 }},

    ci_noop: { pktid: '0x1884', cmdcode: '0' },

    to_noop: { pktid: '0x1880', cmdcode: '0' },
    to_start_telem: { pktid: '0x1880', cmdcode: '6' },

    sample_noop: { pktid: '0x1882', cmdcode: '0' },
    sample_reset_cc: { pktid: '0x1882', cmdcode: '1' },
    sample_toggle: { pktid: '0x1882', cmdcode: '3' },
    sample_file: { pktid: '0x1882', cmdcode: '4', half: [3,1,2,3], endian: 'LE' },
}