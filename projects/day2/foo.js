const fs = require("node:fs");

function lineIsValidUpwards(arr) {
  for (let i = 1; i < arr.length; i++) {
    const a = arr[i - 1];
    const b = arr[i];
    const diff = b - a;
    if (diff < 1 || diff > 3) return false;
  }
  return true;
}

function lineIsValidDownwards(arr) {
  for (let i = 1; i < arr.length; i++) {
    const a = arr[i - 1];
    const b = arr[i];
    const diff = a - b;
    if (diff < 1 || diff > 3) return false;
  }
  return true;
}

function lineIsValid(arr) {
  return lineIsValidUpwards(arr) || lineIsValidDownwards(arr);
}

function lineIsValidByRemovingAny(arr) {
  const mutants = arr.map((x, idx) => {
    const copy = [...arr];
    copy.splice(idx, 1);
    return copy;
  });

  return mutants.some(lineIsValid);
}

const lines = fs
  .readFileSync("./inputs/input.txt", "utf-8")
  .split("\n")
  .map((line) => line.split(" ").map((str) => parseInt(str, 10)));

const validLines = lines.filter((line) => {
  return lineIsValid(line) || lineIsValidByRemovingAny(line);
});

console.log(validLines.length);
