const telemetry = {
    cfe_es: {
            full_name: 'cFE (ES)',
            application_id: 0,
            def: [
                { show: true, name: 'Command Counter', offset: 0, size: 1, display: 10 },
                { show: true, name: 'Command Error Counter', offset: 1, size: 1, display: 10 },
                { show: true, name: 'Checksum', offset: 2, size: 2, display: 16 },
                { show: true, name: 'cFE Major Version', offset: 4, size: 1, display: 10 },
                { show: true, name: 'cFE Minor Version', offset: 5, size: 1, display: 10 },
                { show: true, name: 'cFE Revision', offset: 6, size: 1, display: 10 },
                { show: true, name: 'cFE Mission Revision', offset: 7, size: 1, display: 10 },
                { show: true, name: 'OSAL Major Version', offset: 8, size: 1, display: 10 },
                { show: false, name: 'OSAL Minor Version', offset: 9, size: 1, display: 10 },
                { show: false, name: 'OSAL Revision', offset: 10, size: 1, display: 10 },
                { show: false, name: 'OSAL Mission Revision', offset: 11, size: 1, display: 10 },
                { show: false, name: 'Sys Log Bytes Used', offset: 12, size: 4, display: 10 },
                { show: false, name: 'Sys Log Bytes Total', offset: 16, size: 4, display: 10 },
                { show: false, name: 'Sys Log Entries', offset: 20, size: 4, display: 10 },
                { show: false, name: 'Sys Log Mode', offset: 24, size: 4, display: 10 },
                { show: false, name: 'Error Log Index', offset: 28, size: 4, display: 10 },
                { show: false, name: 'Error Log Entries', offset: 32, size: 4, display: 10 },
                { show: true, name: 'Core Apps', offset: 36, size: 4, display: 10 },
                { show: true, name: 'External Apps', offset: 40, size: 4, display: 10 },
                { show: true, name: 'Tasks', offset: 44, size: 4, display: 10 },
                { show: true, name: 'Libraries', offset: 48, size: 4, display: 10 },
                { show: true, name: 'Reset Type', offset: 52, size: 4, display: 10 },
                { show: true, name: 'Reset Subtype', offset: 56, size: 4, display: 10 },
                { show: true, name: 'Processor Resets', offset: 60, size: 4, display: 10 },
                { show: true, name: 'Max Resets', offset: 64, size: 4, display: 10 },
                { show: true, name: 'Boot Source', offset: 68, size: 4, display: 10 }, 
                { show: true, name: 'Perf State', offset: 72, size: 4, display: 10 },
                { show: true, name: 'Perf Mode', offset: 76, size: 4, display: 10 },
                { show: true, name: 'Perf Trigger Count', offset: 80, size: 4, display: 10 },
                { show: false, name: 'Per Filter Mask', offset: 84, size: 4, display: 16 },
                { show: false, name: 'Perf Trigger Mask', offset: 88, size: 4, display: 16 },
                { show: false, name: 'Perf Data Start', offset: 92, size: 4, display: 10 },
                { show: false, name: 'Perf Data End', offset: 96, size: 4, display: 10 },
                { show: false, name: 'Perf Data Count', offset: 100, size: 4, display: 10 },
                { show: false, name: 'Perf Data To Write', offset: 104, size: 4, display: 10 },
                { show: true, name: 'Heap Bytes Free', offset: 108, size: 4, display: 10 },
                { show: true, name: 'Heap Blocks Free', offset: 112, size: 4, display: 10 },
                { show: true, name: 'Heap Max Block Size', offset: 116, size: 4, display: 10 },
        ]
    },

    cfe_evs: {
        full_name: 'cFE (EVS)',
        application_id: 1,
        def: [
            { show: true, name: 'Command Counter', offset: 0, size: 1, display: 10 },
            { show: true, name: 'Command Error Counter', offset: 1, size: 1, display: 10 },
            { show: true, name: 'Message Format Mode', offset: 2, size: 1, display: 10 },
            { show: true, name: 'Message Trunc Counter', offset: 3, size: 1, display: 10 },
            { show: true, name: 'Unregistered Apps', offset: 4, size: 1, display: 10 },
            { show: true, name: 'Output Port', offset: 5, size: 1, display: 10 },
            { show: true, name: 'Log Full Flag', offset: 6, size: 1, display: 10 },
            { show: true, name: 'Log Mode', offset: 7, size: 1, display: 10 },
            { show: true, name: 'Message Send Counter', offset: 8, size: 2, display: 10 },
            { show: true, name: 'Log Overflow Counter', offset: 10, size: 2, display: 10 },
            { show: true, name: 'Log Enabled', offset: 12, size: 1, display: 10 },
            { show: true, name: 'Spare 1', offset: 13, size: 1, display: 10 },
            { show: true, name: 'Spare 2', offset: 14, size: 1, display: 10 },
            { show: true, name: 'Spare 3', offset: 15, size: 1, display: 10 },
        ]
    },

    cfe_tbl: {
        full_name: 'cFE (TBL)',
        application_id: 4,
        def: [
            { show: true, name: 'Command Counter', offset: 0, size: 1, display: 10 },
            { show: true, name: 'Command Error Counter', offset: 1, size: 1, display: 10 },
            { show: true, name: 'Num Tables', offset: 2, size: 2, display: 10 },
            { show: true, name: 'Num Load Pending', offset: 4, size: 2, display: 10 },
            { show: true, name: 'Validation Counter', offset: 6, size: 2, display: 10 },
            { show: true, name: 'Last CRC', offset: 8, size: 4, display: 10 },
            { show: true, name: 'Last Status', offset: 12, size: 4, display: 10 },
            { show: true, name: 'Active Buffer', offset: 16, size: 1, display: 10 },
            { show: true, name: 'Last Name', offset: 17, size: 40, display: 'ascii' }, // size from CFE_MISSION_TBL_MAX_FULL_NAME_LEN
            { show: true, name: 'Success Validation Counter', offset: 0, size: 1, display: 10 },
            { show: true, name: 'Fail Validation Counter', offset: 0, size: 1, display: 10 },
            { show: true, name: 'Num Validation Requests', offset: 0, size: 1, display: 10 },
        ]
    },

    cfe_sb: {
        full_name: 'cFE (SB)',
        application_id: 3,
        def: [
            { show: true, name: 'Command Counter', offset: 0, size: 1, display: 10 },
            { show: true, name: 'Command Error Counter', offset: 1, size: 1, display: 10 },
            { show: true, name: 'No Subs Counter', offset: 2, size: 1, display: 10 },
            { show: true, name: 'Message Send Error', offset: 3, size: 1, display: 10 },
            { show: true, name: 'Message Receive Error', offset: 4, size: 1, display: 10 },
            { show: true, name: 'Internal Error Counter', offset: 5, size: 1, display: 10 },
            { show: true, name: 'Create Pipe Error Counter', offset: 6, size: 1, display: 10 },
            { show: true, name: 'Sub Error Counter', offset: 7, size: 1, display: 10 },
            { show: true, name: 'Pipe Opts Error Counter', offset: 8, size: 1, display: 10 },
            { show: true, name: 'Dupe Subs Error', offset: 9, size: 1, display: 10 },
            { show: true, name: 'GetPipeIdByName Error Counter', offset: 10, size: 1, display: 10 },
            { show: true, name: 'Spare2Align', offset: 11, size: 1, display: 10 },
            { show: true, name: 'Pipe Overflow Error Counter', offset: 12, size: 2, display: 10 },
            { show: true, name: 'Message Limit Error Counter', offset: 14, size: 2, display: 10 },
            { show: true, name: 'Mem Pool Handle', offset: 16, size: 4, display: 10 },
            { show: true, name: 'Mem In Use', offset: 20, size: 4, display: 10 },
            { show: true, name: 'Unmarked Mem', offset: 24, size: 4, display: 10 },
        ]
    },

    cfe_time: {
        full_name: 'cFE (TIME)',
        application_id: 5,
        def: [
            { show: true, name: 'Command Counter', offset: 0, size: 1, display: 10 },
            { show: true, name: 'Command Error Counter', offset: 1, size: 1, display: 10 },
            { show: true, name: 'Clock State Flags', offset: 2, size: 2, display: 10 },
            { show: true, name: 'Clock State API', offset: 4, size: 2, display: 10 },
            { show: true, name: 'Leap Seconds', offset: 6, size: 2, display: 10 },
            { show: true, name: 'Seconds MET', offset: 8, size: 4, display: 10 },
            { show: true, name: 'Subsecs MET', offset: 12, size: 4, display: 10 },
            { show: true, name: 'Seconds STCF', offset: 16, size: 4, display: 10 },
            { show: true, name: 'Subsecs STCF', offset: 20, size: 4, display: 10 },
        ]
    },

    sample_app: {
        full_name: 'Sample App',
        application_id: 131,
        def: [
            { show: true, name: 'Command Error Counter', offset: 0, size: 1, display: 10 },
            { show: true, name: 'Command Counter', offset: 1, size: 1, display: 10 },
            { show: true, name: 'LED Status', offset: 2, size: 1, display: 10 },
    ]
}
}