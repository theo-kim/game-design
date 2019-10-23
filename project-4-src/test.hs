myzip => [a] -> [b] -> [(a, b)]
myzip [] _ = []
myzip _ [] = []
myzip [] [] = []
myzip [a] [b] = 
