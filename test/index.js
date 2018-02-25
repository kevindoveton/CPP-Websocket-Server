const key = "dGhlIHNhbXBsZSBub25jZQ==258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
const crypto = require('crypto');

const hash = crypto.createHash('sha1')
  .update(key, 'binary')
  .digest('base64');

console.log(hash)