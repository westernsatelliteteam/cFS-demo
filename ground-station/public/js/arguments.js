const args = {
    cfe_evs_noop: { pktid: '0x1801' },
    cfe_sb_noop: { pktid: '0x1803' },
    cfe_tbl_noop: { pktid: '0x1804' },
    cfe_time_noop: { pktid: '0x1805' },
    cfe_es_noop: { pktid: '0x1806' },
    sample_toggle: { pktid: '0x1882', cmdcode: '3' },
    sample_file: { pktid: '0x1882', cmdcode: '4', half: [3,1,2,3], endian: 'LE' }
}