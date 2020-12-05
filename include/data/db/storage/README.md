# Storage Management & Buffer

## Page & Disk

### Mapping

`Page`s are a form of abstraction of `Disk Block`s, so that buffer management could be more convinient. Any `Page` corresponds to a continuous region of a `Disk Block`. Consecutive pages do not necessarily correspond to physically consecutive disk blocks, which matters not, for as long as the mapping relation is clear, a disk block can rightly find their pages, and vice versa.

In our implementation, a page manager is set a `begin offset` and `page size`. `begin offset` is a coordinate to a disk block. The consecutive space in the following disk blocks are mapped to this page manager. The beginning coordinate of each page in the page manager can be trivially computed with given `page size`.

### Access

When a page in a page manager is to be accessed, read or written to, it means that a disk block is to be accessed, and vice versa.

The consecutive steps of accessing a page, or a region of disk block, is:

- Find meta data in memory with a page handle.
- Prepare a buffer.
    - If there is already a buffer mapped.
        - Varify the mapping relation. If not valid, remap.
        - If valid, the buffer still contains the content of this page, or disk block region. Use it directly.
    - If there is no mapped buffer, request the Buffer Manager for a mapping creation.
- Perform access operation.

### Caution

## Buffer

When accessing disk blocks, they must be loaded into memory in the form of Buffers.

A buffer manager provides the following service:
- Create new mapping between a buffer handle and a page handle. (Allocate)
- Break an existing mapping between a buffer handle and a page handle. (Evict)

### Create Mapping

- Free buffers are managed by a free list. A free list is a stack, therefore, popping gives the most-recently put back buffer.
- If there is no free list, an existing mapping must be broken.

### Break Mapping

Policy:
- A mapping must not be broken if a page is currently `using` it.
- The `least-used` page is to be first evicted.

Operation:
- Determine the buffer handle with an LRU list.
- Pop the handle from the list, preventing another thread trying to evict it.
- Request the page manager to flush the content to the disk block.

### Caution

These services require that a page manager must serve soley a single page manager.


