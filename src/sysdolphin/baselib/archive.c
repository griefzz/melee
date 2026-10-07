#include "archive.h"

#include <string.h>

#include <dolphin/os.h>
#ifdef PORT
#include <port/archive_extern.h> // port_archive_extern_declined()
#include <port/dat.h>
#include <port/dat.h> // port_dat_transcode(), port_dat_locate_extern()
#endif

static inline void Locate(HSD_Archive* archive)
{
    u32 i;
    u32* ptr;

    for (i = 0; i < archive->header.nb_reloc; i++) {
        ptr = (u32*) (archive->data + archive->reloc_info[i].offset);
        *ptr += (uintptr_t) archive->data;
    }
}

s32 HSD_ArchiveParse(HSD_Archive* archive, u8* src, size_t file_size)
{
#ifdef PORT
    // PORT: replaced whole. The header is big-endian, and the archive's
    // pointers are four bytes where the host's are eight, so relocating in
    // place cannot work. The transcoder walks the archive from its root
    // symbols, writes a native object graph and returns an HSD_Archive whose
    // pointers are already absolute, which leaves Locate() nothing to do.
    // See docs/design/dat-transcoder.md.
    return port_dat_transcode(archive, src, file_size);
#else
    u32 offset;

    if (archive == NULL) {
        return -1;
    }

    memset(archive, 0, sizeof(HSD_Archive));
    archive->flags |= 1;
    memcpy(archive, src, sizeof(HSD_ArchiveHeader));

    if (archive->header.file_size != file_size) {
        OSReport("HSD_ArchiveParse: byte-order mismatch! Please check data "
                 "format %x %x\n",
                 archive->header.file_size, file_size);
        return -1;
    }

    offset = sizeof(HSD_ArchiveHeader);
    if (archive->header.data_size != 0) { // Body Size
        archive->data = src + sizeof(HSD_ArchiveHeader);
        offset = archive->header.data_size + sizeof(HSD_ArchiveHeader);
    }
    if (archive->header.nb_reloc != 0) { // Relocation Size
        archive->reloc_info = (HSD_ArchiveRelocationInfo*) (src + offset);
        offset = offset +
                 archive->header.nb_reloc * sizeof(HSD_ArchiveRelocationInfo);
    }
    if (archive->header.nb_public != 0) { // Root Size
        archive->public_info = (HSD_ArchivePublicInfo*) (src + offset);
        offset =
            offset + archive->header.nb_public * sizeof(HSD_ArchivePublicInfo);
    }
    if (archive->header.nb_extern != 0) { // XRef Size
        archive->extern_info = (HSD_ArchiveExternInfo*) (src + offset);
        offset =
            offset + archive->header.nb_extern * sizeof(HSD_ArchiveExternInfo);
    }
    if (offset < archive->header.file_size) { // File Size
        archive->symbols = (char*) (src + offset);
    }

    archive->top_ptr = src;
    Locate(archive);

    return 0;
#endif
}

void* HSD_ArchiveGetPublicAddress(HSD_Archive* archive, const char* symbols)
{
    u32 i;

    for (i = 0; i < archive->header.nb_public; i++) {
        int comparison =
            strcmp(archive->symbols + archive->public_info[i].symbol, symbols);

        if (comparison == 0) {
            // If both strings are equal, we've found the node
            return archive->data + archive->public_info[i].offset;
        }
    }

    return NULL;
}

char* HSD_ArchiveGetExtern(HSD_Archive* archive, int index)
{
    if (index < 0 || archive->header.nb_extern <= (u32) index) {
        return NULL;
    }

    return archive->symbols + archive->extern_info[index].symbol;
}

void HSD_ArchiveLocateExtern(HSD_Archive* archive, const char* symbols,
                             void* addr)
{
#ifdef PORT
    // PORT: the chain is threaded through the file's data block, each slot
    // holding the console offset of the next, and `archive->data` is the
    // transcoder's native image, where the slots are eight-byte pointers at
    // other offsets. Walked here, the offsets index nothing and the walk does
    // not end. The transcoder flattens each chain as it converts, and this
    // writes through the native slots it recorded. See
    // docs/design/dat-transcoder.md, "Extern chains".
    {
        int n = port_dat_locate_extern(archive, symbols, addr);

        if (n < 0) {
            // PORT: the chains were never flattened (the archive did not
            // come through the transcoder, or the registry was full). Counted
            // rather than guessed at, since none patched and none looked for
            // are different answers.
            port_archive_extern_declined(symbols);
        }
    }
    return;
#else
    u32 offset = -1U;
    u32 i;

    for (i = 0; i < archive->header.nb_extern; i++) {
        int comparison =
            strcmp(symbols, archive->symbols + archive->extern_info[i].symbol);

        if (comparison == 0) {
            offset = archive->extern_info[i].offset;
            break;
        }
    }

    if (offset == -1U) {
        return;
    }

    while (offset != -1U && offset < archive->header.data_size) {
        u32* slot = (u32*) (archive->data + offset);
        u32 next = *slot;
        *slot = (uintptr_t) addr;
        offset = next;
    }
#endif
}
