/* Default destination MAC address for PTP */
#define CONFIG_PTP_DEFAULT_DST_ADDRESS {0x01, 0x1b, 0x19, 0x00, 0x00, 0x00}

static bool
preparePacketBuffer(SVPublisher self, CommParameters* parameters, const char* interfaceId, bool useVlanTags)
{
    uint8_t defaultDstAddr[] = CONFIG_PTP_DEFAULT_DST_ADDRESS;

    uint8_t* dstAddr;
    uint8_t priority;
    uint16_t vlanId;
    uint16_t appId;

    uint8_t srcAddr[6];

    if (interfaceId != NULL)
        Ethernet_getInterfaceMACAddress(interfaceId, srcAddr);
    else
        Ethernet_getInterfaceMACAddress(CONFIG_ETHERNET_INTERFACE_ID, srcAddr);

    if (parameters == NULL)
    {
        dstAddr = defaultDstAddr;
        priority = CONFIG_SV_DEFAULT_PRIORITY;
        vlanId = CONFIG_SV_DEFAULT_VLAN_ID;
        appId = CONFIG_SV_DEFAULT_APPID;
    }
    else
    {
        dstAddr = parameters->dstAddress;
        priority = parameters->vlanPriority;
        vlanId = parameters->vlanId;
        appId = parameters->appId;
    }if

    if (interfaceId != NULL)
        self->ethernetSocket = Ethernet_createSocket(interfaceId, dstAddr);
    else
        self->ethernetSocket = Ethernet_createSocket(CONFIG_ETHERNET_INTERFACE_ID, dstAddr);

    if (self->ethernetSocket == NULL)
    {
        if (DEBUG_SV_PUBLISHER)
            printf("SV_PUBLISHER: Failed to allocate Ethernet interface\n");

        return false;
    }

    self->buffer = (uint8_t*)GLOBAL_MALLOC(SV_MAX_MESSAGE_SIZE);

    if (self->buffer)
    {
        memcpy(self->buffer, dstAddr, 6);
        memcpy(self->buffer + 6, srcAddr, 6);

        int bufPos = 12;

        if (useVlanTags)
        {
            /* Priority tag - IEEE 802.1Q */
            self->buffer[bufPos++] = 0x81;
            self->buffer[bufPos++] = 0x00;

            uint8_t tci1 = priority << 5;
            tci1 += vlanId / 256;

            uint8_t tci2 = vlanId % 256;

            self->buffer[bufPos++] = tci1; /* Priority + VLAN-ID */
            self->buffer[bufPos++] = tci2; /* VLAN-ID */
        }

        /* EtherType PTP */
        self->buffer[bufPos++] = 0x88;
        self->buffer[bufPos++] = 0xF7;

        /* PTP v2 */
        self->buffer[bufPos++] = 0x00;
        self->buffer[bufPos++] = 0x02;

        self->lengthField = bufPos;

        /* Length */
        self->buffer[bufPos++] = 0x00;
        self->buffer[bufPos++] = 0x2C;

        /* Domain */
        self->buffer[bufPos++] = 0x00;

        /* Reserved */
        self->buffer[bufPos++] = 0x00;

        /* Flags */
        self->buffer[bufPos++] = 0x00;
        self->buffer[bufPos++] = 0x00;

        /* Correction ns - test value */
        self->buffer[bufPos++] = 0x00;
        self->buffer[bufPos++] = 0x01;
        self->buffer[bufPos++] = 0x02;
        self->buffer[bufPos++] = 0x03;      
        self->buffer[bufPos++] = 0x00;
        self->buffer[bufPos++] = 0x01;
        self->buffer[bufPos++] = 0x02;
        self->buffer[bufPos++] = 0x03;

        /* Reserved */
        self->buffer[bufPos++] = 0x00;
        self->buffer[bufPos++] = 0x00;
        self->buffer[bufPos++] = 0x00;
        self->buffer[bufPos++] = 0x00;

        /* Clock identtity */
        self->buffer[bufPos++] = 0x00;
        self->buffer[bufPos++] = 0x01;
        self->buffer[bufPos++] = 0x02;
        self->buffer[bufPos++] = 0x03;      
        self->buffer[bufPos++] = 0x00;
        self->buffer[bufPos++] = 0x01;
        self->buffer[bufPos++] = 0x02;
        self->buffer[bufPos++] = 0x03;

        /* Clock port */
        self->buffer[bufPos++] = 0x00;
        self->buffer[bufPos++] = 0x01;
 
        /* Sequence */
        self->buffer[bufPos++] = 0x00;
        self->buffer[bufPos++] = 0x2C;

        /* Control field - legacy - same as message type */
        self->buffer[bufPos++] = 0x00;

        /* Log interval (1s) */
        self->buffer[bufPos++] = 0x00;
        self->payloadStart = bufPos;
    }
    else
    {
        return false;
    }

    return true;
}
